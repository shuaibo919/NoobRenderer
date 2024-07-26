#pragma once
#include "Graphics/RHI/DescriptorSet.h"

namespace pluto
{
    namespace Graphics
    {
        class VKShader;
        class UniformBuffer;
        class VKDescriptorSet : public DescriptorSet
        {
            friend class VKContext;

        public:
            VKDescriptorSet(RenderContext *ctx, VKDescriptorSet::Properties *&&pProperties);
            ~VKDescriptorSet();

        public:
            void Bind(uint32_t offset = 0);
            void Update(SharedPtr<CommandBuffer> buffer = nullptr) override;
            void SetTexture(const std::string &name, const SharedPtr<Texture> &texture, AttachmentType textureType = AttachmentType::Color, uint32_t mipIndex = 0) override;
            void SetBuffer(const std::string &name, const SharedPtr<UniformBuffer> &buffer) override;
            SharedPtr<UniformBuffer> GetUniformBuffer(const std::string &name) override;
            void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data) override;
            void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size) override;
            void SetUniformBufferData(const std::string &bufferName, void *data) override;
            void TransitionImages(SharedPtr<CommandBuffer> commandBuffer = nullptr) {}
            void SetUniformDynamic(const std::string &bufferName, uint32_t size) override {}
            Buffer *GetUniformBufferLocalData(const std::string &name) override { return nullptr; }
        };
    }
}
