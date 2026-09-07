#include "upstream/img-info.hpp"
#include <cstdio>
#include <string>
#include <vector>
bool load_print_stages(const std::string &, std::vector<SigfmImgInfo> *, std::string *);
int main(int argc, char **argv)
{
    if (argc < 2) return 2;
    std::vector<SigfmImgInfo> st; std::string err;
    if (!load_print_stages(argv[1], &st, &err)) { std::printf("FAIL: %s\n", err.c_str()); return 1; }
    std::printf("%zu stages\n", st.size());
    for (size_t i = 0; i < st.size(); i++)
        std::printf("  stage %2zu: %4zu keypoints, descriptors %dx%d\n",
                    i, st[i].keypoints.size(), st[i].descriptors.rows, st[i].descriptors.cols);
    return 0;
}
