#include "Preprocess.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string GetContent(const std::string &file, const std::string &dir);
nlohmann::json GetSpirv(std::string content);

int main(int argc, char *argv[])
{
    const char *shader_directory = argv[1];
    const char *output_directory = argv[2];
    const std::string mapping[4] = {"Vertex", "Fragment", "Geometry", "Compute"};
    LoadShaderFiles::Init();
    std::cout << "Preprocess shader files in: " << shader_directory << std::endl;
    std::cout << "Output shader files in: " << output_directory << std::endl;
    // Todo: convert and collect glsl to generate .shader.json files
    for (auto &shader : ShaderFilesMap)
    {
        nlohmann::json j;
        std::vector<std::string> glsl_contents;
        std::vector<std::string> shader_names;
        shader_names.push_back(std::get<0>(shader.second));
        shader_names.push_back(std::get<1>(shader.second));
        shader_names.push_back(std::get<2>(shader.second));
        shader_names.push_back(std::get<3>(shader.second));
        for (int i = 0; i < 4; i++)
        {
            auto content = GetContent(shader_names[i], shader_directory);
            if (content.empty())
                continue;
            j[mapping[i]]["type"] = mapping[i];
            j[mapping[i]]["name"] = shader_names[i];
            j[mapping[i]]["glsl"] = content;
            j[mapping[i]]["spirv"] = GetSpirv(content); // todo
        }
        std::ofstream o(output_directory + shader.first + ".shader.json");
        o << j << std::endl;
    }

    return 0;
}
std::string GetContent(const std::string &file, const std::string &dir)
{
    if (file.empty())
        return "";
    auto filepath = dir + file;
    std::ifstream ifs(filepath);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

nlohmann::json GetSpirv(std::string content)
{
    // todo
    return nlohmann::json();
}