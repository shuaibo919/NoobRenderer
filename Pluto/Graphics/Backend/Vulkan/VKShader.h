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
            void Bind() const override;
            void Unbind() const override;
            bool IsCompiled() override;

        private:
            bool LoadSpriv(const std::string &name, uint32_t *source, uint32_t fileSize, ShaderType shaderType, int currentShaderStage);

        private:
            bool mCompiled;
            uint8_t mStageCount;
            VkPipelineLayout mPipelineLayout;
            VkPipelineShaderStageCreateInfo *mShaderStages;
            std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
        };
    }
}
