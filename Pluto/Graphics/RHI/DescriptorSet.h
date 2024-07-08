#pragma once
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    struct Buffer;
    namespace Graphics
    {
        class DescriptorSet
        {
        public:
            using Ptr = std::shared_ptr<DescriptorSet>;
            struct Properties
            {
            };
            struct Builder final : BuildBase<DescriptorSet::Properties, DescriptorSet>
            {
                Builder() {}
                DescriptorSet::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~DescriptorSet();

            // virtual void Update(std::shared_ptr<CommandBuffer> cmdBuffer = nullptr) = 0;
            // virtual void SetDynamicOffset(uint32_t offset) = 0;
            // virtual uint32_t GetDynamicOffset() const = 0;
            // virtual void SetTexture(const std::string &name, Texture **texture, uint32_t textureCount, TextureType textureType = TextureType(0)) = 0;
            // virtual void SetTexture(const std::string &name, Texture *texture, uint32_t mipIndex = 0, TextureType textureType = TextureType(0)) = 0;
            // virtual void SetBuffer(const std::string &name, UniformBuffer *buffer) = 0;
            // virtual std::shared_ptr<UniformBuffer> GetUniformBuffer(const std::string &name) = 0;
            // virtual void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data) = 0;
            // virtual void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size) = 0;
            // virtual void SetUniformBufferData(const std::string &bufferName, void *data) = 0;
            virtual void TransitionImages(std::shared_ptr<CommandBuffer> commandBuffer = nullptr) {}
            virtual void SetUniformDynamic(const std::string &bufferName, uint32_t size) {}
            virtual Buffer *GetUniformBufferLocalData(const std::string &name) { return nullptr; }

        protected:
            Properties *mProperties;
            DescriptorSet(Properties *&&pProperties);
        };
    }
}
