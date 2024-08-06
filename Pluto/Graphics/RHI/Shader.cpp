#include "Graphics/RHI/Shader.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;

Shader::Builder &Shader::Builder::SetFile(const std::string &filePath)
{
    mProperties->filePath = filePath;
    return *this;
}

Shader::Ptr Shader::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
{
    return pContext->CreateShader(std::move(mProperties));
}

Shader::Shader(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

Shader::~Shader()
{
    if (mProperties != nullptr)
        delete mProperties;
}

void Shader::ReadReflectInfo(ShaderJson &info, ShaderType type)
{

    for (auto &vertexInput : info["VertexInput"])
    {
        auto inputType = static_cast<ShaderDataType>(vertexInput);
    }

    for (auto &resource : info["SampledImages"])
    {
        auto &descriptorInfo = mProperties->reflectInfo[resource["set"].get<uint32_t>()];
        auto &descriptor = descriptorInfo.descriptors.emplace_back();
        descriptor.binding = resource["binding"].get<uint32_t>();
        descriptor.name = resource["name"].get<std::string>();
        descriptor.shaderType = static_cast<ShaderType>(resource["shaderType"].get<uint8_t>());
        descriptor.descType = DescriptorType::ImageSampler;
    }

    for (auto &uniform_buffer : info["UniformBuffers"])
    {
        auto &descriptorInfo = mProperties->reflectInfo[uniform_buffer["set"].get<uint32_t>()];
        auto &descriptor = descriptorInfo.descriptors.emplace_back();
        descriptor.offset = 0;
        descriptor.ubo = nullptr;
        descriptor.size = 0;
        descriptor.descType = DescriptorType::UniformBuffer;
        descriptor.name = uniform_buffer["name"].get<std::string>();
        descriptor.binding = uniform_buffer["binding"].get<uint32_t>();
        descriptor.shaderType = static_cast<ShaderType>(uniform_buffer["shaderType"].get<uint8_t>());
        for (auto &json_member : uniform_buffer["members"])
        {
            auto &member = descriptor.mMembers.emplace_back();
            member.size = json_member["size"].get<uint32_t>();
            descriptor.size += member.size;
            member.offset = json_member["offset"].get<uint32_t>();
            member.type = static_cast<ShaderDataType>(json_member["type"].get<uint8_t>());
            member.fullName = json_member["fullname"].get<std::string>();
            member.name = json_member["name"].get<std::string>();
        }
    }

    for (auto &pushConstant : info["PushConstants"])
    {
        // Todo
    }

    int imageCount[16] = {0};
    int bufferCount[16] = {0};

    for (int i = 0; i < mProperties->reflectInfo.size(); i++)
    {
        auto &descriptorInfo = mProperties->reflectInfo[i];
        for (auto &descriptor : descriptorInfo.descriptors)
        {
            if (descriptor.descType == DescriptorType::ImageSampler)
            {
                imageCount[i]++;

                if (i > 0)
                    descriptor.binding = descriptor.binding + imageCount[i - 1];
            }
            else if (descriptor.descType == DescriptorType::UniformBuffer)
            {
                bufferCount[i]++;

                if (i > 0)
                    descriptor.binding = descriptor.binding + bufferCount[i - 1];
            }
        }
    }
}