#include "Preprocess.h"
#include <iostream>
#include <fstream>
#include <sstream>
int main(int argc, char *argv[])
{
    const char *shader_directory = argv[1];
    const char *output_directory = argv[2];
    LoadShaderFiles::Init();
    std::cout << "Preprocess shader files in: " << shader_directory << std::endl;
    // Todo: convert and collect glsl to generate .shader.json files
    for (auto &shader : ShaderFilesMap)
    {
        // auto vert_file = shader_directory + std::get<1>(shader.second);
        // auto frag_file = shader_directory + std::get<1>(shader.second);
        // std::ifstream ifs1(vert_file), ifs2(frag_file);
        // std::ostringstream oss1, oss2;
        // oss1 << ifs1.rdbuf();
        // oss2 << ifs2.rdbuf();
        // std::string vertContent = oss1.str();
        // std::string fragContent = oss2.str();
        // std::cout << vertContent << std::endl;
        // std::cout << fragContent << std::endl;
        // next step: read glsl and generate .shader.json files
    }
    return 0;
}