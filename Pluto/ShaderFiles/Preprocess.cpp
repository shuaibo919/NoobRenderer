#include "Preprocess.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "spirv_cross.hpp"
#include "spirv_glsl.hpp"
#include "Graphics/RHI/Declarations.h"
#include "Core/FileSystem.h"

std::string ReadShaderContent(const std::string &file, const std::string &dir);
nlohmann::json ReflectFromSpirv(std::vector<uint32_t> spv, std::string type, std::string &toGLSL);
std::vector<uint32_t> CompileToSpirv(std::string exec, std::string name, std::string type, std::string shaderfile, std::string output_directory);

int main(int argc, char *argv[])
{
    const char *shader_directory = argv[1];
    const char *output_directory = argv[2];
    const char *glslc = argv[3];
    const std::string mapping[4] = {"Vertex", "Fragment", "Geometry", "Compute"}; // this relative order is important
    LoadShaderFiles::Init();
    std::cout << "Preprocess shader files in: " << shader_directory << std::endl;
    std::cout << "Output shader files in: " << output_directory << std::endl;
    std::cout << "glslc: " << glslc << std::endl;
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
            auto content = ReadShaderContent(shader_names[i], shader_directory);
            if (content.empty())
                continue;
            j[mapping[i]]["type"] = i;
            j[mapping[i]]["name"] = shader_names[i];
            std::string glsl_content;
            auto spirv = CompileToSpirv(glslc, shader_names[i], mapping[i], shader_directory + shader_names[i], output_directory);
            j[mapping[i]]["spirv"] = shader_names[i] + ".spv";
            j[mapping[i]]["reflection"] = ReflectFromSpirv(spirv, mapping[i], glsl_content);
            j[mapping[i]]["glsl"] = glsl_content;
        }
        std::ofstream o(output_directory + shader.first + ".shader.json");
        o << j << std::endl;
    }

    return 0;
}

std::string ReadShaderContent(const std::string &file, const std::string &dir)
{
    if (file.empty())
        return "";
    auto filepath = dir + file;
    std::ifstream ifs(filepath);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

std::string GetGlslcShaderType(std::string type)
{
    if (type == "Vertex")
        return "vertex";
    if (type == "Fragment")
        return "fragment";
    if (type == "Geometry")
        return "geometry";
    if (type == "Compute")
        return "compute";
    return "";
}

std::vector<uint32_t> CompileToSpirv(std::string exec, std::string name, std::string type, std::string shaderfile, std::string output_directory)
{
    std::string stype = GetGlslcShaderType(type);
    std::string output = output_directory + name + ".spv";
    std::string command = exec + " -fshader-stage=" + stype + " -o " + output + " " + shaderfile;
    std::system(command.c_str());
    // read spirv
    auto fileSize = pluto::FileSystem::GetFileSize(output); // TODO: once process
    uint32_t *source = reinterpret_cast<uint32_t *>(FileSystem::ReadFile(output));
    std::vector<unsigned int> buffer(source, source + fileSize / sizeof(unsigned int));
    return buffer;
}

pluto::Graphics::ShaderDataType VertexToShaderDataType(const spirv_cross::SPIRType type)
{
    using namespace pluto::Graphics;
    switch (type.basetype)
    {
    case spirv_cross::SPIRType::Float:
        switch (type.vecsize)
        {
        case 1:
            return ShaderDataType::Float32;

            break;
        case 2:
            return ShaderDataType::Vec2;
            break;
        case 3:
            return ShaderDataType::Vec3;
            break;
        case 4:
            return ShaderDataType::Vec4;
            break;
        }
    case spirv_cross::SPIRType::Double:
        break;
    default:
        break;
    }
    return ShaderDataType::None;
}

pluto::Graphics::ShaderDataType SPIRVTypeToDataType(const spirv_cross::SPIRType type)
{
    using namespace pluto::Graphics;
    switch (type.basetype)
    {
    case spirv_cross::SPIRType::Boolean:
        return ShaderDataType::Bool;
    case spirv_cross::SPIRType::Int:
        if (type.vecsize == 1)
            return ShaderDataType::Int;
        if (type.vecsize == 2)
            return ShaderDataType::Ivec2;
        if (type.vecsize == 3)
            return ShaderDataType::Ivec3;
        if (type.vecsize == 4)
            return ShaderDataType::Ivec4;

    case spirv_cross::SPIRType::UInt:
        return ShaderDataType::Uint;
    case spirv_cross::SPIRType::Float:
        if (type.columns == 3)
            return ShaderDataType::Mat3;
        if (type.columns == 4)
            return ShaderDataType::Mat4;

        if (type.vecsize == 1)
            return ShaderDataType::Float32;
        if (type.vecsize == 2)
            return ShaderDataType::Vec2;
        if (type.vecsize == 3)
            return ShaderDataType::Vec3;
        if (type.vecsize == 4)
            return ShaderDataType::Vec4;
        break;
    case spirv_cross::SPIRType::Struct:
        return ShaderDataType::Struct;
    default:
        break;
    }
    return ShaderDataType::None;
}

nlohmann::json ReflectFromSpirv(std::vector<uint32_t> spv, std::string type, std::string &toGLSL)
{
    nlohmann::json j;
    spirv_cross::CompilerGLSL *glsl = new spirv_cross::CompilerGLSL(std::move(spv));

    // The SPIR-V is now parsed, and we can perform reflection on it.
    spirv_cross::ShaderResources resources = glsl->get_shader_resources();

    if (type == "Vertex")
    {
        std::vector<uint8_t> VertexInput;
        for (const spirv_cross::Resource &resource : resources.stage_inputs)
        {
            const spirv_cross::SPIRType &InputType = glsl->get_type(resource.type_id);
            VertexInput.push_back(static_cast<uint8_t>(VertexToShaderDataType(InputType)));
        }
        j["VertexInput"] = VertexInput;
    }

    std::vector<nlohmann::json> SampledImages;
    for (auto &resource : resources.sampled_images)
    {
        uint32_t set = glsl->get_decoration(resource.id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl->get_decoration(resource.id, spv::DecorationBinding);
        nlohmann::json tmp_j;
        tmp_j["set"] = set;
        tmp_j["binding"] = binding;
        tmp_j["name"] = resource.name;
        tmp_j["shaderType"] = type;
        SampledImages.push_back(tmp_j);
    }
    j["SampledImages"] = SampledImages;

    std::vector<nlohmann::json> UniformBuffers;
    for (auto const &uniform_buffer : resources.uniform_buffers)
    {
        auto set{glsl->get_decoration(uniform_buffer.id, spv::Decoration::DecorationDescriptorSet)};
        glsl->set_decoration(uniform_buffer.id, spv::Decoration::DecorationDescriptorSet, 2 + 2 * set);

        uint32_t binding = glsl->get_decoration(uniform_buffer.id, spv::DecorationBinding);
        auto &bufferType = glsl->get_type(uniform_buffer.type_id);

        auto bufferSize = glsl->get_declared_struct_size(bufferType);
        int memberCount = (int)bufferType.member_types.size();

        nlohmann::json tmp_j;
        tmp_j["set"] = set;
        tmp_j["binding"] = binding;
        tmp_j["name"] = uniform_buffer.name;
        tmp_j["shaderType"] = type;
        tmp_j["offset"] = 0;
        tmp_j["members"] = nlohmann::json();
        for (int i = 0; i < memberCount; ++i)
        {
            nlohmann::json member;
            auto type = glsl->get_type(bufferType.member_types[i]);
            const auto &memberName = glsl->get_member_name(bufferType.self, i);
            auto size = glsl->get_declared_struct_member_size(bufferType, i);
            auto offset = glsl->type_struct_member_offset(bufferType, i);
            std::string uniformName = uniform_buffer.name + "." + memberName;

            member["fullname"] = uniformName;
            member["name"] = memberName;
            member["offset"] = offset;
            member["type"] = SPIRVTypeToDataType(type);
            member["size"] = (uint32_t)size;
            tmp_j["members"].push_back(member);
        }

        UniformBuffers.push_back(tmp_j);
    }
    j["UniformBuffers"] = UniformBuffers;

    std::vector<nlohmann::json> PushConstantBuffers;
    for (auto &u : resources.push_constant_buffers)
    {
        uint32_t set = glsl->get_decoration(u.id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl->get_decoration(u.id, spv::DecorationBinding);
        glsl->set_decoration(u.id, spv::DecorationBinding, 64);

        auto &pushConstantType = glsl->get_type(u.type_id);
        auto name = glsl->get_name(u.id);

        auto ranges = glsl->get_active_buffer_ranges(u.id);
        uint32_t rangeSizes = 0;
        for (auto &range : ranges)
        {
            rangeSizes += uint32_t(range.range);
        }

        auto &bufferType = glsl->get_type(u.base_type_id);
        auto bufferSize = glsl->get_declared_struct_size(bufferType);
        int memberCount = (int)bufferType.member_types.size();
        nlohmann::json tmp_j;
        tmp_j["set"] = set;
        tmp_j["binding"] = binding;
        tmp_j["name"] = name;
        tmp_j["shaderType"] = type;
        tmp_j["rangeSizes"] = rangeSizes;
        tmp_j["members"] = nlohmann::json();
        for (int i = 0; i < memberCount; i++)
        {
            nlohmann::json member;
            auto type = glsl->get_type(bufferType.member_types[i]);
            const auto &memberName = glsl->get_member_name(bufferType.self, i);
            auto size = glsl->get_declared_struct_member_size(bufferType, i);
            auto offset = glsl->type_struct_member_offset(bufferType, i);

            std::string uniformName = u.name + "." + memberName;

            member["fullname"] = uniformName;
            member["name"] = memberName;
            member["offset"] = offset;
            member["type"] = SPIRVTypeToDataType(type);
            member["size"] = (uint32_t)size;
            tmp_j["members"].push_back(member);
        }
        PushConstantBuffers.push_back(tmp_j);
    }
    j["PushConstantBuffers"] = PushConstantBuffers;

    spirv_cross::CompilerGLSL::Options options;
    options.version = 410;
    options.es = false;
    options.vulkan_semantics = false;
    options.separate_shader_objects = false;
    options.enable_420pack_extension = false;
    options.emit_push_constant_as_uniform_buffer = true;
    options.vertex.fixup_clipspace = true;
    glsl->set_common_options(options);
    toGLSL = glsl->compile();

    return j;
}