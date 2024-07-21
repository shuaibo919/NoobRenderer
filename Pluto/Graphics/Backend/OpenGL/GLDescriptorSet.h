#pragma once
#include "Graphics/RHI/DescriptorSet.h"

namespace pluto
{
    namespace Graphics
    {
        class GLShader;
        class UniformBuffer;
        class GLDescriptorSet : public DescriptorSet
        {
            friend class GLContext;

        public:
            GLDescriptorSet(RenderContext *ctx, GLDescriptorSet::Properties *&&pProperties);
            ~GLDescriptorSet();

        public:
            void Bind(uint32_t offset = 0);
            void Update(std::shared_ptr<CommandBuffer> buffer = nullptr) override;
            void SetTexture(const std::string &name, std::shared_ptr<Texture> texture, AttachmentType textureType = AttachmentType::Color, uint32_t mipIndex = 0) override;
            void SetBuffer(const std::string &name, std::shared_ptr<UniformBuffer> buffer) override;
            std::shared_ptr<UniformBuffer> GetUniformBuffer(const std::string &name) override;
            void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data) override;
            void SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size) override;
            void SetUniformBufferData(const std::string &bufferName, void *data) override;
            void TransitionImages(std::shared_ptr<CommandBuffer> commandBuffer = nullptr) {}
            void SetUniformDynamic(const std::string &bufferName, uint32_t size) {}
            Buffer *GetUniformBufferLocalData(const std::string &name) { return nullptr; }

        public:
            struct UniformBufferInfo
            {
                std::shared_ptr<UniformBuffer> ubo;
                std::vector<BufferMemberInfo> members;
                uint8_t *data{nullptr};
                uint32_t size{0};
                bool updated{false};
            };

        private:
            std::unordered_map<std::string, UniformBufferInfo> mUniformBuffers;
            void AllocateUboInfoData(UniformBufferInfo &info, uint32_t size);
            void ReleaseUboInfoData(UniformBufferInfo &info);
            void WrtieUboInfoData(UniformBufferInfo &info, void *data, uint32_t size, uint32_t offset = 0);
        };
    }
}
