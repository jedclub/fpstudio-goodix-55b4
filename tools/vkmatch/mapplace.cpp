#include "mapplace.h"

#include <vulkan/vulkan.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <stdexcept>

namespace vkmap {
namespace {
constexpr uint32_t kBindings = 5;
constexpr uint32_t kChunk = 65536;

void check(VkResult result, const char *operation) {
    if (result != VK_SUCCESS)
        throw std::runtime_error(std::string(operation) + " failed: " + std::to_string(int(result)));
}
struct PackedPose { float offsetX, offsetY, cosine, sine; };
static_assert(sizeof(PackedPose) == 16 && sizeof(Result) == 8);
}

struct Placer::Impl {
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physical = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
    uint32_t family = 0, timestampBits = 0;
    VkPhysicalDeviceProperties properties{};
    VkPhysicalDeviceMemoryProperties memoryProperties{};
    VkDescriptorSetLayout descriptorLayout = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSet descriptors = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer command = VK_NULL_HANDLE;
    VkFence fence = VK_NULL_HANDLE;
    VkQueryPool queries = VK_NULL_HANDLE;

    struct Buffer {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        void *mapped = nullptr;
        VkDeviceSize size = 0;
        bool coherent = false;
    };
    Buffer mapPixels, mapWeights, probe, poses, results;
    uint32_t mapSize = 0, width = 0, height = 0;
    bool submissionIncomplete = false;
    double wallMs = 0, gpuMs = -1;

    ~Impl() {
        if (device) {
            vkDeviceWaitIdle(device);
            destroy(mapPixels); destroy(mapWeights); destroy(probe);
            destroy(poses); destroy(results);
            if (queries) vkDestroyQueryPool(device, queries, nullptr);
            if (fence) vkDestroyFence(device, fence, nullptr);
            if (commandPool) vkDestroyCommandPool(device, commandPool, nullptr);
            if (descriptorPool) vkDestroyDescriptorPool(device, descriptorPool, nullptr);
            if (pipeline) vkDestroyPipeline(device, pipeline, nullptr);
            if (pipelineLayout) vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
            if (descriptorLayout) vkDestroyDescriptorSetLayout(device, descriptorLayout, nullptr);
            vkDestroyDevice(device, nullptr);
        }
        if (instance) vkDestroyInstance(instance, nullptr);
    }
    void destroy(Buffer &b) {
        if (b.mapped) vkUnmapMemory(device, b.memory);
        if (b.buffer) vkDestroyBuffer(device, b.buffer, nullptr);
        if (b.memory) vkFreeMemory(device, b.memory, nullptr);
        b = {};
    }
    Buffer makeBuffer(VkDeviceSize size) {
        if (size == 0 || size > properties.limits.maxStorageBufferRange)
            throw std::runtime_error("Storage buffer exceeds device range");
        Buffer b; b.size = size;
        try {
            VkBufferCreateInfo info{}; info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            info.size = size; info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            check(vkCreateBuffer(device, &info, nullptr, &b.buffer), "vkCreateBuffer");
            VkMemoryRequirements req{}; vkGetBufferMemoryRequirements(device, b.buffer, &req);
            int selected = -1;
            for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
                auto flags = memoryProperties.memoryTypes[i].propertyFlags;
                if ((req.memoryTypeBits & (1u << i)) && (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
                    selected = int(i);
                    if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) break;
                }
            }
            if (selected < 0) throw std::runtime_error("GPU has no host-visible storage buffer memory");
            b.coherent = (memoryProperties.memoryTypes[selected].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;
            VkMemoryAllocateInfo allocation{}; allocation.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocation.allocationSize = req.size; allocation.memoryTypeIndex = uint32_t(selected);
            check(vkAllocateMemory(device, &allocation, nullptr, &b.memory), "vkAllocateMemory");
            check(vkBindBufferMemory(device, b.buffer, b.memory, 0), "vkBindBufferMemory");
            check(vkMapMemory(device, b.memory, 0, VK_WHOLE_SIZE, 0, &b.mapped), "vkMapMemory");
            return b;
        } catch (...) { destroy(b); throw; }
    }
    void flush(const Buffer &b, bool invalidate = false) {
        if (b.coherent) return;
        VkMappedMemoryRange range{}; range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range.memory = b.memory; range.offset = 0; range.size = VK_WHOLE_SIZE;
        check(invalidate ? vkInvalidateMappedMemoryRanges(device, 1, &range)
                         : vkFlushMappedMemoryRanges(device, 1, &range),
              invalidate ? "vkInvalidateMappedMemoryRanges" : "vkFlushMappedMemoryRanges");
    }

    void initialize(const std::string &path) {
        std::ifstream input(path, std::ios::binary | std::ios::ate);
        if (!input) throw std::runtime_error("Cannot open compute SPIR-V: " + path);
        auto length = input.tellg();
        if (length < 20 || length > 16 * 1024 * 1024 || length % 4)
            throw std::runtime_error("Invalid compute SPIR-V size");
        std::vector<uint32_t> code(size_t(length) / 4); input.seekg(0);
        if (!input.read(reinterpret_cast<char *>(code.data()), length) || code[0] != 0x07230203)
            throw std::runtime_error("Invalid compute SPIR-V data");

        VkApplicationInfo app{}; app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app.pApplicationName = "fpstudio-map-placement-research"; app.apiVersion = VK_API_VERSION_1_1;
        VkInstanceCreateInfo ii{}; ii.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; ii.pApplicationInfo = &app;
        check(vkCreateInstance(&ii, nullptr, &instance), "vkCreateInstance (Vulkan 1.1 required)");
        uint32_t count = 0;
        check(vkEnumeratePhysicalDevices(instance, &count, nullptr), "vkEnumeratePhysicalDevices");
        if (!count) throw std::runtime_error("No Vulkan GPU found");
        std::vector<VkPhysicalDevice> devices(count);
        check(vkEnumeratePhysicalDevices(instance, &count, devices.data()), "vkEnumeratePhysicalDevices");
        // The same rule the matcher applies: real graphics hardware, unless a
        // test opts into a software device explicitly.
        const char *softwareVulkan = std::getenv("FPSTUDIO_ALLOW_SOFTWARE_VULKAN");
        const bool allowSoftware = softwareVulkan && std::strcmp(softwareVulkan, "1") == 0;
        int best = -1;
        for (auto candidate : devices) {
            VkPhysicalDeviceProperties props{}; vkGetPhysicalDeviceProperties(candidate, &props);
            const bool graphicsHardware = props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
                                          props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
            const bool ciSoftwareDevice = allowSoftware && props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU;
            if (!graphicsHardware && !ciSoftwareDevice) continue;
            if (props.apiVersion < VK_API_VERSION_1_1 ||
                props.limits.maxComputeWorkGroupInvocations < 128 ||
                props.limits.maxComputeWorkGroupSize[0] < 128 ||
                props.limits.maxComputeSharedMemorySize < 6 * 128 * sizeof(float)) continue;
            uint32_t n = 0; vkGetPhysicalDeviceQueueFamilyProperties(candidate, &n, nullptr);
            std::vector<VkQueueFamilyProperties> families(n);
            vkGetPhysicalDeviceQueueFamilyProperties(candidate, &n, families.data());
            for (uint32_t f = 0; f < n; ++f)
                if (families[f].queueCount && (families[f].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
                    const int typeRank = props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 10
                                       : (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? 5 : 1);
                    int rank = typeRank + ((families[f].queueFlags & VK_QUEUE_GRAPHICS_BIT) ? 0 : 1);
                    if (rank > best) {
                        best = rank; physical = candidate; properties = props; family = f;
                        timestampBits = families[f].timestampValidBits;
                    }
                }
        }
        if (!physical) throw std::runtime_error("No compatible physical Vulkan GPU compute queue");
        vkGetPhysicalDeviceMemoryProperties(physical, &memoryProperties);

        float priority = 1;
        VkDeviceQueueCreateInfo qi{}; qi.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qi.queueFamilyIndex = family; qi.queueCount = 1; qi.pQueuePriorities = &priority;
        VkDeviceCreateInfo di{}; di.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        di.queueCreateInfoCount = 1; di.pQueueCreateInfos = &qi;
        check(vkCreateDevice(physical, &di, nullptr, &device), "vkCreateDevice");
        vkGetDeviceQueue(device, family, 0, &queue);

        std::array<VkDescriptorSetLayoutBinding, kBindings> bindings{};
        for (uint32_t i = 0; i < kBindings; ++i) {
            bindings[i].binding = i;
            bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[i].descriptorCount = 1;
            bindings[i].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        }
        VkDescriptorSetLayoutCreateInfo li{}; li.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        li.bindingCount = kBindings; li.pBindings = bindings.data();
        check(vkCreateDescriptorSetLayout(device, &li, nullptr, &descriptorLayout), "vkCreateDescriptorSetLayout");
        VkPushConstantRange push{VK_SHADER_STAGE_COMPUTE_BIT, 0, 20};
        VkPipelineLayoutCreateInfo pi{}; pi.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pi.setLayoutCount = 1; pi.pSetLayouts = &descriptorLayout;
        pi.pushConstantRangeCount = 1; pi.pPushConstantRanges = &push;
        check(vkCreatePipelineLayout(device, &pi, nullptr, &pipelineLayout), "vkCreatePipelineLayout");

        VkShaderModule shader = VK_NULL_HANDLE;
        VkShaderModuleCreateInfo si{}; si.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        si.codeSize = code.size() * 4; si.pCode = code.data();
        check(vkCreateShaderModule(device, &si, nullptr, &shader), "vkCreateShaderModule");
        VkComputePipelineCreateInfo ci{}; ci.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        ci.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        ci.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT; ci.stage.module = shader;
        ci.stage.pName = "main"; ci.layout = pipelineLayout;
        VkResult pipelineResult = vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &ci, nullptr, &pipeline);
        vkDestroyShaderModule(device, shader, nullptr);
        check(pipelineResult, "vkCreateComputePipelines");

        VkDescriptorPoolSize poolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, kBindings};
        VkDescriptorPoolCreateInfo dpi{}; dpi.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        dpi.maxSets = 1; dpi.poolSizeCount = 1; dpi.pPoolSizes = &poolSize;
        check(vkCreateDescriptorPool(device, &dpi, nullptr, &descriptorPool), "vkCreateDescriptorPool");
        VkDescriptorSetAllocateInfo dai{}; dai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        dai.descriptorPool = descriptorPool; dai.descriptorSetCount = 1; dai.pSetLayouts = &descriptorLayout;
        check(vkAllocateDescriptorSets(device, &dai, &descriptors), "vkAllocateDescriptorSets");

        VkCommandPoolCreateInfo cpi{}; cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cpi.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; cpi.queueFamilyIndex = family;
        check(vkCreateCommandPool(device, &cpi, nullptr, &commandPool), "vkCreateCommandPool");
        VkCommandBufferAllocateInfo cai{}; cai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cai.commandPool = commandPool; cai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; cai.commandBufferCount = 1;
        check(vkAllocateCommandBuffers(device, &cai, &command), "vkAllocateCommandBuffers");
        VkFenceCreateInfo fi{}; fi.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        check(vkCreateFence(device, &fi, nullptr, &fence), "vkCreateFence");
        if (timestampBits) {
            VkQueryPoolCreateInfo qpi{}; qpi.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
            qpi.queryType = VK_QUERY_TYPE_TIMESTAMP; qpi.queryCount = 2;
            check(vkCreateQueryPool(device, &qpi, nullptr, &queries), "vkCreateQueryPool");
        }
        poses = makeBuffer(kChunk * sizeof(PackedPose));
        results = makeBuffer(kChunk * sizeof(Result));
    }

    void updateDescriptors() {
        std::array<VkDescriptorBufferInfo, kBindings> info{{
            {mapPixels.buffer, 0, mapPixels.size}, {mapWeights.buffer, 0, mapWeights.size},
            {probe.buffer, 0, probe.size}, {poses.buffer, 0, poses.size},
            {results.buffer, 0, results.size}}};
        std::array<VkWriteDescriptorSet, kBindings> writes{};
        for (uint32_t i = 0; i < kBindings; ++i) {
            writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writes[i].dstSet = descriptors; writes[i].dstBinding = i;
            writes[i].descriptorCount = 1;
            writes[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writes[i].pBufferInfo = &info[i];
        }
        vkUpdateDescriptorSets(device, kBindings, writes.data(), 0, nullptr);
    }
};

Placer::Placer(const std::string &path) : impl(std::make_unique<Impl>()) { impl->initialize(path); }
Placer::~Placer() = default;
std::string Placer::deviceName() const { return impl->properties.deviceName; }
double Placer::lastWallMs() const { return impl->wallMs; }
double Placer::lastGpuMs() const { return impl->gpuMs; }

void Placer::setMap(const std::vector<float> &pixels, const std::vector<float> &weights, uint32_t size) {
    if (size == 0 || size > 8192) throw std::runtime_error("Map size out of range");
    if (pixels.size() != size_t(size) * size || weights.size() != pixels.size())
        throw std::runtime_error("Map buffers do not match the declared size");
    auto &v = *impl;
    const VkDeviceSize bytes = pixels.size() * sizeof(float);
    if (v.mapPixels.size != bytes) {
        vkDeviceWaitIdle(v.device);
        v.destroy(v.mapPixels); v.destroy(v.mapWeights);
        v.mapPixels = v.makeBuffer(bytes); v.mapWeights = v.makeBuffer(bytes);
    }
    std::memcpy(v.mapPixels.mapped, pixels.data(), bytes);
    std::memcpy(v.mapWeights.mapped, weights.data(), bytes);
    v.flush(v.mapPixels); v.flush(v.mapWeights);
    v.mapSize = size;
    if (v.probe.buffer) v.updateDescriptors();
}

void Placer::setProbe(const std::vector<float> &pixels, uint32_t width, uint32_t height) {
    if (width == 0 || height == 0 || pixels.size() != size_t(width) * height)
        throw std::runtime_error("Probe buffer does not match the declared size");
    auto &v = *impl;
    const VkDeviceSize bytes = pixels.size() * sizeof(float);
    if (v.probe.size != bytes) {
        vkDeviceWaitIdle(v.device);
        v.destroy(v.probe);
        v.probe = v.makeBuffer(bytes);
    }
    std::memcpy(v.probe.mapped, pixels.data(), bytes);
    v.flush(v.probe);
    v.width = width; v.height = height;
    if (v.mapPixels.buffer) v.updateDescriptors();
}

std::vector<Result> Placer::run(const std::vector<Pose> &input, uint32_t stride) {
    auto &v = *impl;
    if (v.submissionIncomplete)
        throw std::runtime_error("GPU submission did not complete; recreate the placer before retrying");
    if (!v.mapSize || !v.width) throw std::runtime_error("Upload the map and a probe before placing");
    if (stride == 0 || stride > 64) throw std::runtime_error("Sampling stride must be 1..64");
    const auto started = std::chrono::steady_clock::now();
    std::vector<Result> out(input.size());
    if (v.queries) v.gpuMs = 0; else v.gpuMs = -1;

    const uint32_t maxChunk = std::min(kChunk, v.properties.limits.maxComputeWorkGroupCount[0]);
    for (size_t offset = 0; offset < input.size(); offset += maxChunk) {
        const uint32_t n = uint32_t(std::min<size_t>(maxChunk, input.size() - offset));
        auto *packed = static_cast<PackedPose *>(v.poses.mapped);
        for (uint32_t i = 0; i < n; ++i) {
            const auto &p = input[offset + i];
            const double a = double(p.angleDegrees) * 3.14159265358979323846 / 180.0;
            packed[i] = {p.offsetX, p.offsetY, float(std::cos(a)), float(std::sin(a))};
        }
        v.flush(v.poses);
        check(vkResetCommandBuffer(v.command, 0), "vkResetCommandBuffer");
        VkCommandBufferBeginInfo bi{}; bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        check(vkBeginCommandBuffer(v.command, &bi), "vkBeginCommandBuffer");
        if (v.queries) {
            vkCmdResetQueryPool(v.command, v.queries, 0, 2);
            vkCmdWriteTimestamp(v.command, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, v.queries, 0);
        }
        VkMemoryBarrier before{}; before.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        before.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT; before.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(v.command, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                             0, 1, &before, 0, nullptr, 0, nullptr);
        vkCmdBindPipeline(v.command, VK_PIPELINE_BIND_POINT_COMPUTE, v.pipeline);
        vkCmdBindDescriptorSets(v.command, VK_PIPELINE_BIND_POINT_COMPUTE, v.pipelineLayout,
                                0, 1, &v.descriptors, 0, nullptr);
        std::array<uint32_t, 5> params{v.mapSize, v.width, v.height, stride, n};
        vkCmdPushConstants(v.command, v.pipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, 20, params.data());
        vkCmdDispatch(v.command, n, 1, 1);
        VkMemoryBarrier after{}; after.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        after.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT; after.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
        vkCmdPipelineBarrier(v.command, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_HOST_BIT,
                             0, 1, &after, 0, nullptr, 0, nullptr);
        if (v.queries) vkCmdWriteTimestamp(v.command, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, v.queries, 1);
        check(vkEndCommandBuffer(v.command), "vkEndCommandBuffer");
        check(vkResetFences(v.device, 1, &v.fence), "vkResetFences");
        VkSubmitInfo submit{}; submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.commandBufferCount = 1; submit.pCommandBuffers = &v.command;
        v.submissionIncomplete = true;
        check(vkQueueSubmit(v.queue, 1, &submit, v.fence), "vkQueueSubmit");
        check(vkWaitForFences(v.device, 1, &v.fence, VK_TRUE, 60000000000ull),
              "GPU compute completion (60 second limit)");
        v.submissionIncomplete = false;
        v.flush(v.results, true);
        std::memcpy(out.data() + offset, v.results.mapped, n * sizeof(Result));
        if (v.queries) {
            uint64_t times[2]{};
            check(vkGetQueryPoolResults(v.device, v.queries, 0, 2, sizeof(times), times,
                                        sizeof(uint64_t), VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT),
                  "vkGetQueryPoolResults");
            const uint64_t mask = v.timestampBits >= 64 ? UINT64_MAX : ((uint64_t(1) << v.timestampBits) - 1);
            v.gpuMs += double((times[1] - times[0]) & mask) * v.properties.limits.timestampPeriod / 1e6;
        }
    }
    v.wallMs = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count();
    return out;
}

Result cpuScore(const std::vector<float> &mapPixels, const std::vector<float> &mapWeights,
                uint32_t mapSize, const std::vector<float> &probe, uint32_t width,
                uint32_t height, const Pose &pose, uint32_t stride) {
    // Mirrors the shader's lane-local accumulation and reduction order, so the
    // two agree bit-for-bit closely enough to be compared in a test.
    const double a = double(pose.angleDegrees) * 3.14159265358979323846 / 180.0;
    const float cosine = float(std::cos(a)), sine = float(std::sin(a));
    const uint32_t nx = (width + stride - 1) / stride, ny = (height + stride - 1) / stride;
    const float hw = float(width) * 0.5f, hh = float(height) * 0.5f;
    float red[6][128]{};
    for (uint32_t lane = 0; lane < 128; ++lane)
        for (uint32_t i = lane; i < nx * ny; i += 128) {
            const uint32_t x = (i % nx) * stride, y = (i / nx) * stride;
            const float ddx = float(x) - hw, ddy = float(y) - hh;
            const float rx = cosine * ddx - sine * ddy + hw;
            const float ry = sine * ddx + cosine * ddy + hh;
            const int mx = int(std::lround(pose.offsetX + rx)), my = int(std::lround(pose.offsetY + ry));
            if (mx < 0 || my < 0 || mx >= int(mapSize) || my >= int(mapSize)) continue;
            const size_t idx = size_t(my) * mapSize + size_t(mx);
            if (mapWeights[idx] <= 0) continue;
            const float pa = probe[size_t(y) * width + x], pb = mapPixels[idx];
            red[0][lane] += pa; red[1][lane] += pb; red[2][lane] += pa * pa;
            red[3][lane] += pb * pb; red[4][lane] += pa * pb; red[5][lane] += 1;
        }
    for (uint32_t s = 64; s > 0; s >>= 1)
        for (uint32_t lane = 0; lane < s; ++lane)
            for (uint32_t k = 0; k < 6; ++k) red[k][lane] += red[k][lane + s];
    const float n = red[5][0], total = float(nx * ny);
    Result out;
    if (n < 1) return out;
    out.overlap = n / total;
    const float va = red[2][0] - red[0][0] * red[0][0] / n;
    const float vb = red[3][0] - red[1][0] * red[1][0] / n;
    if (va < 1e-6f || vb < 1e-6f) return out;
    out.ncc = (red[4][0] - red[0][0] * red[1][0] / n) / std::sqrt(va * vb);
    return out;
}
}
