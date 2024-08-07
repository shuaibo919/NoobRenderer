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
    : Shader(ctx, std::move(pProperties)), mStageCount(0)
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

void VKShader::Bind() const
{
}

void VKShader::Unbind() const
{
}

bool VKShader::IsCompiled()
{
    return false;
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
