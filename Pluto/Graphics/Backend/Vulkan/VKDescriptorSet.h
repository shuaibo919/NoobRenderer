#pragma once
#include "Graphics/RHI/DescriptorSet.h"
#include "Graphics/RHI/SwapChain.h"
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKShader;
        class UniformBuffer;
        class VKDescriptorSet : public DescriptorSet, public VKObjectManageByContext
        {
            friend class VKContext;

        public:
            VKDescriptorSet(RenderContext *ctx, VKDescriptorSet::Properties *&&pProperties);
            ~VKDescriptorSet();

        public:
            void DestroyImplementation() override;
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

        public:
            VkDescriptorSet GetHandle() const { return mDescriptorSet; }
            bool GetHasUpdated() const { return mDescriptorUpdated; }
            bool GetDynamic() const { return mDynamic; }
            uint32_t GetDynamicOffset() const { return mDynamicOffset; }

        private:
            bool mDynamic{false};
            uint32_t mDynamicOffset{0};
            struct UniformBufferInfo
            {
                std::vector<BufferMemberInfo> mMembers;
                uint8_t *data{nullptr};
                uint32_t size{0};
                bool dirty{true};
            };
            void AllocateUboInfoData(UniformBufferInfo &info, uint32_t size);
            void ReleaseUboInfoData(UniformBufferInfo &info);
            void WrtieUboInfoData(UniformBufferInfo &info, void *data, uint32_t size, uint32_t offset = 0);
            void TransitionImageLayoutByHints(const SharedPtr<Texture> &texture, const SharedPtr<CommandBuffer> &cmdBuffer);
            VkDescriptorPool mDescriptorPool;
            VkDescriptorSet mDescriptorSet;
            bool mDescriptorDirty;
            bool mDescriptorUpdated;
            std::map<std::string, UniformBufferInfo> mUniformBuffersData;
            std::map<std::string, SharedPtr<UniformBuffer>> mUniformBuffers;
        };
    

    }
}
