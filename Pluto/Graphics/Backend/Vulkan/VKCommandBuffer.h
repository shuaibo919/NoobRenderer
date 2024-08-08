#pragma once
#include "Graphics/RHI/CommandBuffer.h"

#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKFence;
        class VKSemaphore;

        class VKCommandBuffer : public CommandBuffer
        {
        public:
            VKCommandBuffer(RenderContext *ctx, CommandBuffer::Properties *&&pProperties);
            ~VKCommandBuffer();

        public:
            void Submit() override;
            void Reset() override;
            bool Flush() override;
            bool Init(bool primary = true) override;
            bool Init(bool primary, VkCommandPool commandPool);
            void Unload() override;
            void BeginRecording() override;
            void BeginRecordingSecondary(const SharedPtr<RenderPass> &renderPass, const SharedPtr<Framebuffer> &framebuffer) override;
            void EndRecording() override;
            void ExecuteSecondary(const SharedPtr<CommandBuffer> &primaryCmdBuffer) override;

            /* move RenderContext cap to here */
            void Present(const SharedPtr<CommandBuffer> &commandBuffer) override;
            void BindVetexBuffer(const SharedPtr<Pipeline> &pipeline, const SharedPtr<VertexBuffer> &vbo, uint8_t binding = 0) override;
            void BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet) override;
            void BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets) override;
            void UpdateViewport(uint32_t width, uint32_t height, bool flipViewport = false) override;
            void BindPipeline(const SharedPtr<Pipeline> &pipeline) override;
            void BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer) override;
            void DrawIndexed(DrawType type, uint32_t count, uint32_t start) override;
            void Draw(DrawType type, uint32_t count) override;
            void Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) override;
            void DrawSplashScreen(const SharedPtr<Texture> &texture) override;
            void UnBindPipeline() override;
            void EndCurrentRenderPass() override;

        public:
            VkCommandBuffer GetHandle() const { return mCommandBuffer; }

        private:
            bool mPrimary{false};
            SharedPtr<VKFence> mFence{nullptr};
            SharedPtr<VKSemaphore> mSemaphore{nullptr};
            VkCommandPool mCommandPool{VK_NULL_HANDLE};
            VkCommandBuffer mCommandBuffer{VK_NULL_HANDLE};

            uint32_t mBoundPipelineLayer{0};
        };
    }
}
