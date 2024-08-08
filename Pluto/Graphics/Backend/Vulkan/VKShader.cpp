/* Vulkan Shader */
#include "Graphics/Backend/Vulkan/VKShader.h"
/* Usage */
#include "Core/FileSystem.h"
#include "Core/Utilities.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;

VKShader::VKShader(RenderContext *ctx, VKShader::Properties *&&pProperties)
    : Shader(ctx, std::move(pProperties)), mStageCount(0), mCompiled(false)
{
    if (!mProperties->filePath.empty())
    {
        std::string realpath = FileSystem::Instance().GetPhysicalPath(mProperties->filePath);
        std::string dirPath = FileSystem::Instance().GetParentPath(realpath);
        std::string jsonContent = FileSystem::Instance().ReadTextFile(realpath);
        ShaderJson shaderDesc = ShaderJson::parse(jsonContent);

        mShaderStages = new VkPipelineShaderStageCreateInfo[shaderDesc.size()];

        for (uint32_t i = 0; i < shaderDesc.size(); i++)
            mShaderStages[i] = VkPipelineShaderStageCreateInfo();

        bool success = true;
        for (auto &shader : shaderDesc)
        {
            auto type = shader["type"].get<ShaderType>();
            auto spvName = shader["spirv"].get<std::string>();
            auto spvPath = dirPath + "/" + spvName;

            uint32_t fileSize = uint32_t(FileSystem::GetFileSize(spvPath));
            uint32_t *source = reinterpret_cast<uint32_t *>(FileSystem::Instance().ReadFile(spvPath));

            success &= this->LoadSpriv(spvName, source, fileSize, type, mStageCount);

            delete[] source;

            mProperties->types.push_back(type);
            this->ReadReflectInfo(shader["reflection"], type);
            mStageCount++;
        }
        this->PreparePipelineLayout();
        mCompiled = success;
    }
    else
    {
        PLog<PError>("[%s] Vulkan Shader: No file path provided", PLineInfo);
    }
}

VKShader::~VKShader()
{
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    for (uint32_t i = 0; i < mStageCount; i++)
        vkDestroyShaderModule(pBasedDevice->GetDevice(), mShaderStages[i].module, nullptr);

    delete[] mShaderStages;

    // todo
    mStageCount = 0;
}

bool VKShader::LoadSpriv(const std::string &name, uint32_t *source, uint32_t fileSize, ShaderType shaderType, int currentShaderStage)
{
    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    auto pBasedDevice = pRenderContext->GetBasedDevice();
    VkShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize = fileSize;
    shaderCreateInfo.pCode = source;
    shaderCreateInfo.pNext = VK_NULL_HANDLE;

    mShaderStages[currentShaderStage].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    mShaderStages[currentShaderStage].stage = VKUtilities::GetShaderType(shaderType);
    mShaderStages[currentShaderStage].pName = "main";
    mShaderStages[currentShaderStage].pNext = VK_NULL_HANDLE;

    VkResult result = vkCreateShaderModule(pBasedDevice->GetDevice(), &shaderCreateInfo, nullptr, &mShaderStages[currentShaderStage].module);

    std::string debugName = name + ":" + Utilities::GetShaderTypeString(shaderType);
    VKUtilities::SetDebugUtilsObjectNameInfo(pRenderContext->GetVKInstance(), pBasedDevice->GetDevice(), VK_OBJECT_TYPE_SHADER_MODULE, debugName.c_str(), mShaderStages[currentShaderStage].module);
    VK_CHECK_RESULT(result);

    if (result == VK_SUCCESS)
    {
        return true;
    }
    return false;
}

void VKShader::ReadReflectInfo(ShaderJson &info, ShaderType type)
{
    uint32_t maxSetNum = info["max_set"].get<uint32_t>();
    mDescriptorLayoutInfos.resize(maxSetNum);
    for (auto &vertexInput : info["VertexInput"])
    {
        auto inputType = static_cast<ShaderDataType>(vertexInput);
    }

    for (auto &resource : info["SampledImages"])
    {
        uint32_t set = resource["set"].get<uint32_t>();
        auto &descriptorInfo = mProperties->reflectInfo[set];
        auto &descriptor = descriptorInfo.descriptors.emplace_back();
        descriptor.binding = resource["binding"].get<uint32_t>();
        descriptor.name = resource["name"].get<std::string>();
        descriptor.shaderType = static_cast<ShaderType>(resource["shaderType"].get<uint8_t>());
        descriptor.descType = DescriptorType::ImageSampler;
        mDescriptorLayoutInfos[set].push_back({DescriptorType::ImageSampler,
                                               type,
                                               descriptor.binding,
                                               set,
                                               resource["per_dimension_size"].get<std::vector<uint32_t>>()});
    }

    for (auto &uniform_buffer : info["UniformBuffers"])
    {
        uint32_t set = uniform_buffer["set"].get<uint32_t>();
        uint32_t dimension = uniform_buffer["dimension"].get<uint32_t>();
        auto &descriptorInfo = mProperties->reflectInfo[set];
        auto &descriptor = descriptorInfo.descriptors.emplace_back();
        descriptor.offset = 0;
        descriptor.ubo = nullptr;
        descriptor.size = 0;
        descriptor.descType = DescriptorType::UniformBuffer;
        descriptor.name = uniform_buffer["name"].get<std::string>();
        descriptor.binding = uniform_buffer["binding"].get<uint32_t>();
        descriptor.shaderType = static_cast<ShaderType>(uniform_buffer["shaderType"].get<uint8_t>());
        mDescriptorLayoutInfos[set].push_back({DescriptorType::UniformBuffer,
                                               type,
                                               descriptor.binding,
                                               set,
                                               uniform_buffer["per_dimension_size"].get<std::vector<uint32_t>>()});
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
}

void VKShader::PreparePipelineLayout()
{
    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    auto pBasedDevice = pRenderContext->GetBasedDevice();

    for (auto &l : mDescriptorLayoutInfos)
    {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
        std::vector<VkDescriptorBindingFlags> layoutBindingFlags;
        setLayoutBindings.reserve(l.size());
        layoutBindingFlags.reserve(l.size());

        for (uint32_t i = 0; i < (uint32_t)l.size(); i++)
        {
            auto &info = l[i];

            int foundIndex = -1;
            for (uint32_t i = 0; i < (uint32_t)setLayoutBindings.size(); i++)
            {
                if (setLayoutBindings[i].binding == info.binding)
                    foundIndex = i;
            }

            VkDescriptorSetLayoutBinding &setLayoutBinding = foundIndex >= 0 ? setLayoutBindings[foundIndex] : setLayoutBindings.emplace_back();

            setLayoutBinding.descriptorType = VKUtilities::GetDescriptorType(info.type);
            if (foundIndex >= 0)
            {
                setLayoutBinding.stageFlags |= VKUtilities::GetShaderType(info.stage);
            }
            else
                setLayoutBinding.stageFlags = VKUtilities::GetShaderType(info.stage);

            setLayoutBinding.binding = info.binding;
            setLayoutBinding.descriptorCount = info.counts.size();

            bool isArray = info.counts.size() > 1;

            if (foundIndex < 0)
                layoutBindingFlags.push_back(isArray ? VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT : 0);
        }

        VkDescriptorSetLayoutBindingFlagsCreateInfo flagsInfo = {};
        flagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
        flagsInfo.pNext = nullptr;
        flagsInfo.bindingCount = static_cast<uint32_t>(layoutBindingFlags.size());
        flagsInfo.pBindingFlags = layoutBindingFlags.data();

        // Pipeline layout
        VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo = {};
        setLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        setLayoutCreateInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        setLayoutCreateInfo.pBindings = setLayoutBindings.data();
        setLayoutCreateInfo.pNext = &flagsInfo;

        VkDescriptorSetLayout layout;
        vkCreateDescriptorSetLayout(pBasedDevice->GetDevice(), &setLayoutCreateInfo, VK_NULL_HANDLE, &layout);

        mDescriptorSetLayouts.push_back(layout);
    }

    // Todo Push Constant

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(mDescriptorSetLayouts.size());
    pipelineLayoutCreateInfo.pSetLayouts = mDescriptorSetLayouts.data();
    // pipelineLayoutCreateInfo.pushConstantRangeCount = ;
    // pipelineLayoutCreateInfo.pPushConstantRanges = ;

    VK_CHECK_RESULT(vkCreatePipelineLayout(pBasedDevice->GetDevice(), &pipelineLayoutCreateInfo, VK_NULL_HANDLE, &mPipelineLayout));
}