#pragma once
#include "Graphics/RHI/Shader.h"

#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKShader : public Shader
        {
            friend class VKContext;

        public:
            VKShader(RenderContext *ctx, VKShader::Properties *&&pProperties);
            ~VKShader();

        public:
            void Bind() const override {};
            void Unbind() const override {};
            bool IsCompiled() override { return mCompiled; }

        public:
            bool HasComputeStage() const { return mCompute; }
            uint8_t GetStageCount() const { return mStageCount; }
            VkPipelineLayout GetPipelineLayout() const { return mPipelineLayout; }
            VkPipelineShaderStageCreateInfo *GetShaderStages() const { return mShaderStages; }

        private:
            void ReadReflectInfo(ShaderJson &info, ShaderType type) override;
            bool LoadSpriv(const std::string &name, uint32_t *source, uint32_t fileSize, ShaderType shaderType, int currentShaderStage);
            void PreparePipelineLayout();

        private:
            bool mCompiled;
            bool mCompute;
            uint8_t mStageCount;
            VkPipelineLayout mPipelineLayout;
            VkPipelineShaderStageCreateInfo *mShaderStages;
            std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
            std::vector<std::vector<DescriptorLayoutInfo>> mDescriptorLayoutInfos;

            uint32_t mVertexInputStride{0};
            std::vector<VkVertexInputAttributeDescription> mVertexInputAttributeDescriptions;
        };
    }
}
