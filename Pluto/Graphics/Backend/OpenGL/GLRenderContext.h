#pragma once
#include "Graphics/RHI/Declarations.h"
#include "Graphics/RHI/RenderContext.h"

namespace pluto
{
    namespace Graphics
    {
        class GLContext;
        class GLSwapChain;
        class GLRenderContext : public RenderContext
        {
        public:
            struct State
            {
                /* Depth Setting */
                bool EnableDepthTest{false};
                bool EnableDepthWrite{false};
                /* Cull Setting */
                bool EnableCull{false};
                CullMode CtxCullMode{CullMode::Back};
                /* Blend Mode */
                bool EnableBlend{false};
                BlendMode CtxBlendMode{BlendMode::None};
                /* Stencil Mode */
                bool EnableStencil{false};
                StencilType CtxStencilType{StencilType::None};
            };

            State state;
            GLRenderContext(GLContext *ctx);
            ~GLRenderContext();
            static void Clear(uint32_t buffer);

        public:
            void Init() override;
            void Begin() override;
            void WaitIdle() override;
            void OnResize(uint32_t width, uint32_t height) override;
            void ClearRenderTarget(const SharedPtr<Texture> &texture, AttachmentType type, const SharedPtr<CommandBuffer> &commandBuffer, glm::vec4 clearColor) override;

            void Present() override;
            void Present(const SharedPtr<CommandBuffer> &commandBuffer) override;
            void BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet) override;
            void BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets) override;

            const std::string &GetTitle() const override;
            void DrawIndexed(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count, uint32_t start) const override;
            void Draw(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count) const override;
            void Dispatch(const SharedPtr<CommandBuffer> &cmd, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) override;
            void DrawSplashScreen(const SharedPtr<Texture> &texture) override;
            uint32_t GetGPUCount() const override;
            bool SupportsCompute() override;
            RHIFormat GetDepthFormat() override;

            SwapChain *GetSwapChain() override;

        public:
            /// @brief Because GLRenderContext is not exposed externally,
            /// this part is made public for convenience in gl-backend use
            struct GLObject
            {
                uint32_t handle;
                bool valid;
            };
            inline static GLObject CurrentVertexHandle{0, false};
            inline static GLObject CurrentIndiceHandle{0, false};

            void SetSwapchain(GLSwapChain *swapchain);

        private:
            GLContext *mContext;
            GLSwapChain *mSwapChain{nullptr};
        };
    }
}
