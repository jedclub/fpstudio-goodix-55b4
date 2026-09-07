#include "vkmatch.h"
#include <vulkan/vulkan.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace vkmatch {
namespace {
constexpr uint32_t Chunk=2048;
void check(VkResult result,const char* operation) {
    if(result!=VK_SUCCESS) throw std::runtime_error(std::string(operation)+" failed (Vulkan "+std::to_string(result)+")");
}
void validateImage(const Image& im) {
    if(im.width<3 || im.height<3 || im.width>4096 || im.height>4096 || im.pixels.size()!=size_t(im.width)*im.height)
        throw std::runtime_error("Image dimensions must be 3..4096 and match pixel count");
    for(float v:im.pixels) if(!std::isfinite(v)||v<0||v>1) throw std::runtime_error("Image pixels must be finite grayscale values in [0,1]");
}
void validateJob(const Job& j) {
    if(!std::isfinite(j.angleDegrees)||!std::isfinite(j.dx)||!std::isfinite(j.dy)||!std::isfinite(j.scale)||j.scale<=0||
       !std::isfinite(j.pivotX)||!std::isfinite(j.pivotY)||!std::isfinite(j.anchorX)||!std::isfinite(j.anchorY))
        throw std::runtime_error("Transforms require finite angle/translation and positive finite scale");
    const bool explicitAnchor=j.pivotX>=0&&j.pivotY>=0&&j.anchorX>=0&&j.anchorY>=0;
    if(!usesImageCentreAnchor(j)&&!explicitAnchor)
        throw std::runtime_error("Transform anchor must be a complete nonnegative pivot/anchor pair or the image-centre default");
}
void validateStride(uint32_t stride) { if(stride==0 || stride>4096) throw std::runtime_error("Sampling stride must be 1..4096"); }
struct PackedJob { uint32_t reference,probe; float cosine,sine,dx,dy,scale,reserved,pivotX,pivotY,anchorX,anchorY; };
static_assert(sizeof(PackedJob)==48 && sizeof(Score)==16);
PackedJob pack(const Job& j,uint32_t width,uint32_t height) {
    const double a=double(j.angleDegrees)*3.14159265358979323846/180.0;
    const auto anchor=resolveTransformAnchor(j,width,height);
    return {j.reference,j.probe,float(std::cos(a)),float(std::sin(a)),j.dx,j.dy,j.scale,0,
            anchor.pivotX,anchor.pivotY,anchor.anchorX,anchor.anchorY};
}
}

struct Matcher::Impl {
    VkInstance instance=VK_NULL_HANDLE;
    VkPhysicalDevice physical=VK_NULL_HANDLE;
    VkDevice device=VK_NULL_HANDLE;
    VkQueue queue=VK_NULL_HANDLE;
    uint32_t family=0,timestampBits=0;
    VkPhysicalDeviceProperties properties{};
    VkPhysicalDeviceMemoryProperties memoryProperties{};
    VkDescriptorSetLayout descriptorLayout=VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout=VK_NULL_HANDLE;
    VkPipeline pipeline=VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool=VK_NULL_HANDLE;
    VkDescriptorSet descriptors=VK_NULL_HANDLE;
    VkCommandPool commandPool=VK_NULL_HANDLE;
    VkCommandBuffer command=VK_NULL_HANDLE;
    VkFence fence=VK_NULL_HANDLE;
    VkQueryPool queries=VK_NULL_HANDLE;
    struct Buffer { VkBuffer buffer=VK_NULL_HANDLE; VkDeviceMemory memory=VK_NULL_HANDLE; void* mapped=nullptr; VkDeviceSize size=0; bool coherent=false; };
    Buffer images,jobs,scores;
    uint32_t width=0,height=0,imageCount=0;
    bool submissionIncomplete=false;

    ~Impl() {
        if(device) {
            vkDeviceWaitIdle(device);
            destroy(images); destroy(jobs); destroy(scores);
            if(queries) vkDestroyQueryPool(device,queries,nullptr);
            if(fence) vkDestroyFence(device,fence,nullptr);
            if(commandPool) vkDestroyCommandPool(device,commandPool,nullptr);
            if(descriptorPool) vkDestroyDescriptorPool(device,descriptorPool,nullptr);
            if(pipeline) vkDestroyPipeline(device,pipeline,nullptr);
            if(pipelineLayout) vkDestroyPipelineLayout(device,pipelineLayout,nullptr);
            if(descriptorLayout) vkDestroyDescriptorSetLayout(device,descriptorLayout,nullptr);
            vkDestroyDevice(device,nullptr);
        }
        if(instance) vkDestroyInstance(instance,nullptr);
    }
    void destroy(Buffer& b) {
        if(b.mapped) vkUnmapMemory(device,b.memory);
        if(b.buffer) vkDestroyBuffer(device,b.buffer,nullptr);
        if(b.memory) vkFreeMemory(device,b.memory,nullptr);
        b={};
    }
    Buffer makeBuffer(VkDeviceSize size) {
        if(size==0 || size>properties.limits.maxStorageBufferRange) throw std::runtime_error("Storage buffer exceeds device range");
        Buffer b; b.size=size;
        try {
            VkBufferCreateInfo info{}; info.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            info.size=size; info.usage=VK_BUFFER_USAGE_STORAGE_BUFFER_BIT; info.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
            check(vkCreateBuffer(device,&info,nullptr,&b.buffer),"vkCreateBuffer");
            VkMemoryRequirements req{}; vkGetBufferMemoryRequirements(device,b.buffer,&req);
            int selected=-1;
            for(uint32_t i=0;i<memoryProperties.memoryTypeCount;++i) {
                auto flags=memoryProperties.memoryTypes[i].propertyFlags;
                if((req.memoryTypeBits&(1u<<i)) && (flags&VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
                    selected=int(i); if(flags&VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) break;
                }
            }
            if(selected<0) throw std::runtime_error("GPU has no host-visible storage buffer memory");
            b.coherent=(memoryProperties.memoryTypes[selected].propertyFlags&VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)!=0;
            VkMemoryAllocateInfo allocation{}; allocation.sType=VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocation.allocationSize=req.size; allocation.memoryTypeIndex=uint32_t(selected);
            check(vkAllocateMemory(device,&allocation,nullptr,&b.memory),"vkAllocateMemory");
            check(vkBindBufferMemory(device,b.buffer,b.memory,0),"vkBindBufferMemory");
            check(vkMapMemory(device,b.memory,0,VK_WHOLE_SIZE,0,&b.mapped),"vkMapMemory");
            return b;
        } catch(...) { destroy(b); throw; }
    }
    void flush(const Buffer& b,bool invalidate=false) {
        if(b.coherent) return;
        VkMappedMemoryRange range{}; range.sType=VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range.memory=b.memory; range.offset=0; range.size=VK_WHOLE_SIZE;
        check(invalidate?vkInvalidateMappedMemoryRanges(device,1,&range):vkFlushMappedMemoryRanges(device,1,&range),invalidate?"vkInvalidateMappedMemoryRanges":"vkFlushMappedMemoryRanges");
    }
    void initialize(const std::string& path) {
        std::ifstream input(path,std::ios::binary|std::ios::ate);
        if(!input) throw std::runtime_error("Cannot open compute SPIR-V: "+path);
        auto length=input.tellg();
        if(length<20 || length>16*1024*1024 || length%4) throw std::runtime_error("Invalid compute SPIR-V size");
        std::vector<uint32_t> code(size_t(length)/4); input.seekg(0);
        if(!input.read(reinterpret_cast<char*>(code.data()),length) || code[0]!=0x07230203) throw std::runtime_error("Invalid compute SPIR-V data");
        VkApplicationInfo app{}; app.sType=VK_STRUCTURE_TYPE_APPLICATION_INFO; app.pApplicationName="fpstudio-vkmatch-research"; app.apiVersion=VK_API_VERSION_1_1;
        VkInstanceCreateInfo ii{}; ii.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; ii.pApplicationInfo=&app;
        check(vkCreateInstance(&ii,nullptr,&instance),"vkCreateInstance (Vulkan 1.1 required)");
        uint32_t count=0; check(vkEnumeratePhysicalDevices(instance,&count,nullptr),"vkEnumeratePhysicalDevices");
        if(!count) throw std::runtime_error("No Vulkan GPU found");
        std::vector<VkPhysicalDevice> devices(count); check(vkEnumeratePhysicalDevices(instance,&count,devices.data()),"vkEnumeratePhysicalDevices");
        int best=-1;
        for(auto candidate:devices) {
            VkPhysicalDeviceProperties props{}; vkGetPhysicalDeviceProperties(candidate,&props);
            if(props.deviceType!=VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && props.deviceType!=VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) continue;
            if(props.apiVersion<VK_API_VERSION_1_1 || props.limits.maxComputeWorkGroupInvocations<128 || props.limits.maxComputeWorkGroupSize[0]<128 || props.limits.maxComputeSharedMemorySize<10*128*sizeof(float)) continue;
            uint32_t n=0; vkGetPhysicalDeviceQueueFamilyProperties(candidate,&n,nullptr);
            std::vector<VkQueueFamilyProperties> families(n); vkGetPhysicalDeviceQueueFamilyProperties(candidate,&n,families.data());
            for(uint32_t f=0;f<n;++f) if(families[f].queueCount && (families[f].queueFlags&VK_QUEUE_COMPUTE_BIT)) {
                int rank=(props.deviceType==VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU?10:5)+((families[f].queueFlags&VK_QUEUE_GRAPHICS_BIT)?0:1);
                if(rank>best) { best=rank; physical=candidate; properties=props; family=f; timestampBits=families[f].timestampValidBits; }
            }
        }
        if(!physical) throw std::runtime_error("No compatible physical Vulkan GPU compute queue; CPU/software fallback is disabled");
        vkGetPhysicalDeviceMemoryProperties(physical,&memoryProperties);
        float priority=1;
        VkDeviceQueueCreateInfo qi{}; qi.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; qi.queueFamilyIndex=family; qi.queueCount=1; qi.pQueuePriorities=&priority;
        VkDeviceCreateInfo di{}; di.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; di.queueCreateInfoCount=1; di.pQueueCreateInfos=&qi;
        check(vkCreateDevice(physical,&di,nullptr,&device),"vkCreateDevice"); vkGetDeviceQueue(device,family,0,&queue);
        std::array<VkDescriptorSetLayoutBinding,3> bindings{};
        for(uint32_t i=0;i<3;++i) { bindings[i].binding=i; bindings[i].descriptorType=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; bindings[i].descriptorCount=1; bindings[i].stageFlags=VK_SHADER_STAGE_COMPUTE_BIT; }
        VkDescriptorSetLayoutCreateInfo li{}; li.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO; li.bindingCount=3; li.pBindings=bindings.data();
        check(vkCreateDescriptorSetLayout(device,&li,nullptr,&descriptorLayout),"vkCreateDescriptorSetLayout");
        VkPushConstantRange push{VK_SHADER_STAGE_COMPUTE_BIT,0,16};
        VkPipelineLayoutCreateInfo pi{}; pi.sType=VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO; pi.setLayoutCount=1; pi.pSetLayouts=&descriptorLayout; pi.pushConstantRangeCount=1; pi.pPushConstantRanges=&push;
        check(vkCreatePipelineLayout(device,&pi,nullptr,&pipelineLayout),"vkCreatePipelineLayout");
        VkShaderModule shader=VK_NULL_HANDLE;
        VkShaderModuleCreateInfo si{}; si.sType=VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; si.codeSize=code.size()*4; si.pCode=code.data();
        check(vkCreateShaderModule(device,&si,nullptr,&shader),"vkCreateShaderModule");
        VkComputePipelineCreateInfo ci{}; ci.sType=VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO; ci.stage.sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO; ci.stage.stage=VK_SHADER_STAGE_COMPUTE_BIT; ci.stage.module=shader; ci.stage.pName="main"; ci.layout=pipelineLayout;
        VkResult pipelineResult=vkCreateComputePipelines(device,VK_NULL_HANDLE,1,&ci,nullptr,&pipeline);
        vkDestroyShaderModule(device,shader,nullptr); check(pipelineResult,"vkCreateComputePipelines");
        VkDescriptorPoolSize poolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,3};
        VkDescriptorPoolCreateInfo dpi{}; dpi.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; dpi.maxSets=1; dpi.poolSizeCount=1; dpi.pPoolSizes=&poolSize;
        check(vkCreateDescriptorPool(device,&dpi,nullptr,&descriptorPool),"vkCreateDescriptorPool");
        VkDescriptorSetAllocateInfo dai{}; dai.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO; dai.descriptorPool=descriptorPool; dai.descriptorSetCount=1; dai.pSetLayouts=&descriptorLayout;
        check(vkAllocateDescriptorSets(device,&dai,&descriptors),"vkAllocateDescriptorSets");
        VkCommandPoolCreateInfo cpi{}; cpi.sType=VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; cpi.flags=VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; cpi.queueFamilyIndex=family;
        check(vkCreateCommandPool(device,&cpi,nullptr,&commandPool),"vkCreateCommandPool");
        VkCommandBufferAllocateInfo cai{}; cai.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO; cai.commandPool=commandPool; cai.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY; cai.commandBufferCount=1;
        check(vkAllocateCommandBuffers(device,&cai,&command),"vkAllocateCommandBuffers");
        VkFenceCreateInfo fi{}; fi.sType=VK_STRUCTURE_TYPE_FENCE_CREATE_INFO; check(vkCreateFence(device,&fi,nullptr,&fence),"vkCreateFence");
        if(timestampBits) { VkQueryPoolCreateInfo qpi{}; qpi.sType=VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO; qpi.queryType=VK_QUERY_TYPE_TIMESTAMP; qpi.queryCount=2; check(vkCreateQueryPool(device,&qpi,nullptr,&queries),"vkCreateQueryPool"); }
        jobs=makeBuffer(Chunk*sizeof(PackedJob)); scores=makeBuffer(Chunk*sizeof(Score));
    }
    void updateDescriptors() {
        std::array<VkDescriptorBufferInfo,3> buffers{{{images.buffer,0,images.size},{jobs.buffer,0,jobs.size},{scores.buffer,0,scores.size}}};
        std::array<VkWriteDescriptorSet,3> writes{};
        for(uint32_t i=0;i<3;++i) { writes[i].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; writes[i].dstSet=descriptors; writes[i].dstBinding=i; writes[i].descriptorCount=1; writes[i].descriptorType=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; writes[i].pBufferInfo=&buffers[i]; }
        vkUpdateDescriptorSets(device,3,writes.data(),0,nullptr);
    }
};

Matcher::Matcher(const std::string& path):impl(std::make_unique<Impl>()) { impl->initialize(path); }
Matcher::~Matcher()=default;
std::string Matcher::deviceName() const { return impl->properties.deviceName; }
void Matcher::setImages(const std::vector<Image>& bank) {
    if(impl->submissionIncomplete) throw std::runtime_error("GPU submission did not complete; recreate matcher before uploading");
    if(bank.empty()) throw std::runtime_error("Image bank cannot be empty");
    const auto& first=bank.front();
    for(const auto& im:bank) { validateImage(im); if(im.width!=first.width || im.height!=first.height) throw std::runtime_error("All bank images must share dimensions"); }
    uint64_t pixels=uint64_t(first.width)*first.height*bank.size();
    if(pixels>std::numeric_limits<uint32_t>::max() || pixels>impl->properties.limits.maxStorageBufferRange/sizeof(float)) throw std::runtime_error("Image bank exceeds GPU storage/index limits");
    const auto upload=[&](const Impl::Buffer& buffer) {
        auto* dest=static_cast<float*>(buffer.mapped);
        for(const auto& im:bank) { std::memcpy(dest,im.pixels.data(),im.pixels.size()*sizeof(float)); dest+=im.pixels.size(); }
        impl->flush(buffer);
    };
    if(impl->images.size==pixels*sizeof(float)) {
        // Every dispatch is fence-completed before run() returns. Keep the
        // allocation and descriptors for a persistent streaming image bank.
        // On a failed flush the modified bank must not be used for matching.
        impl->imageCount=0;
        upload(impl->images);
        impl->width=first.width; impl->height=first.height; impl->imageCount=uint32_t(bank.size());
        return;
    }
    auto replacement=impl->makeBuffer(pixels*sizeof(float));
    try {
        upload(replacement);
    } catch(...) { impl->destroy(replacement); throw; }
    impl->destroy(impl->images); impl->images=replacement;
    impl->width=first.width; impl->height=first.height; impl->imageCount=uint32_t(bank.size()); impl->updateDescriptors();
}

Batch Matcher::run(const std::vector<Job>& input,uint32_t stride) {
    if(impl->submissionIncomplete) throw std::runtime_error("GPU submission did not complete; recreate matcher before retrying");
    validateStride(stride);
    if(!impl->imageCount) throw std::runtime_error("Upload images before running matching jobs");
    for(const auto& j:input) {
        validateJob(j);
        if(j.reference>=impl->imageCount || j.probe>=impl->imageCount) throw std::runtime_error("Matching job image index is outside bank");
        if(!usesImageCentreAnchor(j)&&(j.pivotX>=impl->width||j.anchorX>=impl->width||j.pivotY>=impl->height||j.anchorY>=impl->height))
            throw std::runtime_error("Transform pivot/anchor is outside image bounds");
    }
    const auto started=std::chrono::steady_clock::now();
    Batch result; result.scores.resize(input.size()); if(impl->queries) result.gpuMs=0;
    auto& v=*impl;
    uint32_t maxChunk=std::min(Chunk,v.properties.limits.maxComputeWorkGroupCount[0]);
    for(size_t offset=0;offset<input.size();offset+=maxChunk) {
        uint32_t n=uint32_t(std::min<size_t>(maxChunk,input.size()-offset));
        auto* packed=static_cast<PackedJob*>(v.jobs.mapped);
        for(uint32_t i=0;i<n;++i) packed[i]=pack(input[offset+i],v.width,v.height);
        v.flush(v.jobs);
        check(vkResetCommandBuffer(v.command,0),"vkResetCommandBuffer");
        VkCommandBufferBeginInfo bi{}; bi.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO; bi.flags=VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        check(vkBeginCommandBuffer(v.command,&bi),"vkBeginCommandBuffer");
        if(v.queries) { vkCmdResetQueryPool(v.command,v.queries,0,2); vkCmdWriteTimestamp(v.command,VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,v.queries,0); }
        VkMemoryBarrier before{}; before.sType=VK_STRUCTURE_TYPE_MEMORY_BARRIER; before.srcAccessMask=VK_ACCESS_HOST_WRITE_BIT; before.dstAccessMask=VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(v.command,VK_PIPELINE_STAGE_HOST_BIT,VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,0,1,&before,0,nullptr,0,nullptr);
        vkCmdBindPipeline(v.command,VK_PIPELINE_BIND_POINT_COMPUTE,v.pipeline);
        vkCmdBindDescriptorSets(v.command,VK_PIPELINE_BIND_POINT_COMPUTE,v.pipelineLayout,0,1,&v.descriptors,0,nullptr);
        std::array<uint32_t,4> params{v.width,v.height,stride,n};
        vkCmdPushConstants(v.command,v.pipelineLayout,VK_SHADER_STAGE_COMPUTE_BIT,0,16,params.data());
        vkCmdDispatch(v.command,n,1,1);
        VkMemoryBarrier after{}; after.sType=VK_STRUCTURE_TYPE_MEMORY_BARRIER; after.srcAccessMask=VK_ACCESS_SHADER_WRITE_BIT; after.dstAccessMask=VK_ACCESS_HOST_READ_BIT;
        vkCmdPipelineBarrier(v.command,VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,VK_PIPELINE_STAGE_HOST_BIT,0,1,&after,0,nullptr,0,nullptr);
        if(v.queries) vkCmdWriteTimestamp(v.command,VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,v.queries,1);
        check(vkEndCommandBuffer(v.command),"vkEndCommandBuffer");
        check(vkResetFences(v.device,1,&v.fence),"vkResetFences");
        VkSubmitInfo submit{}; submit.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO; submit.commandBufferCount=1; submit.pCommandBuffers=&v.command;
        v.submissionIncomplete=true;
        check(vkQueueSubmit(v.queue,1,&submit,v.fence),"vkQueueSubmit");
        check(vkWaitForFences(v.device,1,&v.fence,VK_TRUE,60000000000ull),"GPU compute completion (60 second limit)");
        v.submissionIncomplete=false;
        v.flush(v.scores,true);
        std::memcpy(result.scores.data()+offset,v.scores.mapped,n*sizeof(Score));
        if(v.queries) {
            uint64_t times[2]{};
            check(vkGetQueryPoolResults(v.device,v.queries,0,2,sizeof(times),times,sizeof(uint64_t),VK_QUERY_RESULT_64_BIT|VK_QUERY_RESULT_WAIT_BIT),"vkGetQueryPoolResults");
            uint64_t mask=v.timestampBits>=64?UINT64_MAX:((uint64_t(1)<<v.timestampBits)-1);
            result.gpuMs+=double((times[1]-times[0])&mask)*v.properties.limits.timestampPeriod/1e6;
        }
    }
    result.wallMs=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-started).count();
    return result;
}

Score cpuScore(const Image& reference,const Image& probe,const Job& job,uint32_t stride) {
    validateImage(reference); validateImage(probe); validateJob(job); validateStride(stride);
    if(reference.width!=probe.width || reference.height!=probe.height) throw std::runtime_error("CPU score images must share dimensions");
    const auto j=pack(job,reference.width,reference.height); const uint32_t w=reference.width,h=reference.height;
    auto at=[&](const Image& im,uint32_t x,uint32_t y) { return im.pixels[size_t(y)*w+x]; };
    auto sample=[&](float x,float y) {
        uint32_t xx=uint32_t(std::floor(x)),yy=uint32_t(std::floor(y));
        float fx=x-xx,fy=y-yy;
        float a=at(probe,xx,yy),b=at(probe,std::min(xx+1,w-1),yy),c=at(probe,xx,std::min(yy+1,h-1)),d=at(probe,std::min(xx+1,w-1),std::min(yy+1,h-1));
        return (a+(b-a)*fx)*(1-fy)+(c+(d-c)*fx)*fy;
    };
    // Mirror the shader's lane-local accumulation and reduction order.
    float sums[10][128]{};
    const uint32_t nx=(w-3)/stride+1,ny=(h-3)/stride+1;
    for(uint32_t lane=0;lane<128;++lane) for(uint32_t i=lane;i<nx*ny;i+=128) {
        uint32_t x=1+(i%nx)*stride,y=1+(i/nx)*stride;
        float dx=x-j.pivotX,dy=y-j.pivotY;
        float px=j.anchorX+j.scale*(j.cosine*dx-j.sine*dy)+j.dx,py=j.anchorY+j.scale*(j.sine*dx+j.cosine*dy)+j.dy;
        if(!(px>=1 && py>=1 && px<=w-2 && py<=h-2)) continue;
        float a=at(reference,x,y),b=sample(px,py);
        float ax=at(reference,x+1,y)-at(reference,x-1,y),ay=at(reference,x,y+1)-at(reference,x,y-1);
        float bx=sample(px+1,py)-sample(px-1,py),by=sample(px,py+1)-sample(px,py-1);
        float rx=j.cosine*bx+j.sine*by,ry=-j.sine*bx+j.cosine*by;
        sums[0][lane]+=a; sums[1][lane]+=b; sums[2][lane]+=a*a; sums[3][lane]+=b*b; sums[4][lane]+=a*b;
        sums[5][lane]+=ax*rx+ay*ry; sums[6][lane]+=ax*ax+ay*ay; sums[7][lane]+=rx*rx+ry*ry; sums[8][lane]+=std::abs(a-b); sums[9][lane]+=1;
    }
    for(uint32_t step=64;step;step/=2) for(uint32_t lane=0;lane<step;++lane) for(uint32_t k=0;k<10;++k) sums[k][lane]+=sums[k][lane+step];
    float n=sums[9][0]; Score out;
    if(n>0) {
        float va=std::max(0.f,sums[2][0]-sums[0][0]*sums[0][0]/n),vb=std::max(0.f,sums[3][0]-sums[1][0]*sums[1][0]/n);
        float gd=std::sqrt(sums[6][0]*sums[7][0]);
        if(va>n*1e-7f && vb>n*1e-7f) out.ncc=std::clamp((sums[4][0]-sums[0][0]*sums[1][0]/n)/std::sqrt(va*vb),-1.f,1.f);
        if(gd>1e-12f) out.gradient=std::clamp(sums[5][0]/gd,-1.f,1.f);
        out.overlap=n/float(nx*ny); out.mae=sums[8][0]/n;
    }
    return out;
}
}
