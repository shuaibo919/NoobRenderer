#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    struct Buffer;
    namespace Graphics
    {
        class DescriptorSet : public RHIBase
        {
        public:
            using Ptr = std::shared_ptr<DescriptorSet>;
            struct Properties
            {
                uint32_t layoutIndex{0};
                std::shared_ptr<Shader> shader{nullptr};
                DescriptorSetInfo descriptorInfo;
            };
            struct Builder final : BuildBase<DescriptorSet::Properties, DescriptorSet>
            {
                Builder() {}
                Builder &SetBindingLayout(std::shared_ptr<Shader> shader, uint32_t index);
                DescriptorSet::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~DescriptorSet();
            const Properties &GetProperties() const { return *mProperties; }

            virtual void Update(std::shared_ptr<CommandBuffer> buffer = nullptr) = 0;
            virtual void SetTexture(const std::string &name, std::shared_ptr<Texture> texture, AttachmentType textureType = AttachmentType::Color, uint32_t mipIndex = 0) = 0;
            virtual void SetBuffer(const std::string &name, std::shared_ptr<UniformBuffer> buffer) = 0;
            virtual std::shared_ptr<UniformBuffer> GetUniformBuffer(const std::string &name) = 0;
            virtual void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data) = 0;
            virtual void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size) = 0;
            virtual void SetUniformBufferData(const std::string &bufferName, void *data) = 0;
            virtual void TransitionImages(std::shared_ptr<CommandBuffer> commandBuffer = nullptr) {}
            virtual void SetUniformDynamic(const std::string &bufferName, uint32_t size) {}
            virtual Buffer *GetUniformBufferLocalData(const std::string &name) { return nullptr; }

        protected:
            Properties *mProperties;
            DescriptorSet(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
