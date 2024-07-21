#pragma once
#include "Graphics/RHI/Declarations.h"
#include "Graphics/RHI/RenderContext.h"

namespace pluto
{
    namespace Graphics
    {
        class GLContext;
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
            void Clear(uint32_t buffer);

        public:
            void Init() override;
            void Begin() override;
            void OnResize(uint32_t width, uint32_t height) override;
            void ClearRenderTarget(std::shared_ptr<Texture> texture, AttachmentType type, std::shared_ptr<CommandBuffer> commandBuffer, glm::vec4 clearColor) override;

            void Present() override;
            void Present(std::shared_ptr<CommandBuffer> commandBuffer) override;
            void BindDescriptorSet(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<CommandBuffer> commandBuffer, uint32_t dynamicOffset, std::shared_ptr<DescriptorSet> descriptorSet) override;
            void BindDescriptorSets(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<CommandBuffer> commandBuffer, uint32_t dynamicOffset, std::vector<std::shared_ptr<DescriptorSet>> descriptorSets) override;

            const std::string &GetTitle() const override;
            void DrawIndexed(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count, uint32_t start) const override;
            void Draw(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count) const override;
            void Dispatch(std::shared_ptr<CommandBuffer>, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) override;
            void DrawSplashScreen(std::shared_ptr<Texture> texture) override;
            uint32_t GetGPUCount() const override;
            bool SupportsCompute() override;
            RHIFormat GetDepthFormat() override;

            std::shared_ptr<SwapChain> GetSwapChain() override;

        public:
            /// @brief Because GLRenderContext is not exposed externally,
            /// this part is made public for convenience in gl-backend use
            struct GLObject
            {
                uint32_t handle;
                bool valid{false};
            };
            GLObject CurrentVertexHandle;
            GLObject CurrentIndiceHandle;

        private:
            GLContext *mContext;
        };
    }
}
