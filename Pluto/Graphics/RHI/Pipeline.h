#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class GraphicsContext;
        class Pipeline : public RHIBase
        {
            friend class CommandBuffer;
            friend class GLCommandBuffer;
            friend class VKCommandBuffer;
            friend class GraphicsContext;

        public:
            using Ptr = SharedPtr<Pipeline>;
            struct Properties
            {
                SharedPtr<Shader> shader;

                CullMode cullMode = CullMode::Back;
                PolygonMode polygonMode = PolygonMode::Fill;
                DrawType drawType = DrawType::Triangle;
                BlendMode blendMode = BlendMode::None;

                bool transparencyEnabled{false};
                bool swapchainTarget{false};
                bool clearTargets{false};

                bool DepthTest{true};
                bool DepthWrite{true};

                std::vector<SharedPtr<Texture>> colorTargets;
                std::vector<Graphics::AttachmentType> attachmentTypes;
                SharedPtr<Texture> resolveTexture = nullptr;
                float clearColor[4]{0.2f, 0.2f, 0.2f, 1.0f};
                float lineWidth{1.0f};
                int mipIndex{0};
                uint8_t samples{1};
            };
            struct Builder final : BuildBase<Pipeline::Properties, Pipeline>
            {
                Builder() {}
                Pipeline::Builder &SetShader(const SharedPtr<Shader> &pShader);
                Pipeline::Builder &SetDrawType(DrawType drawType);
                Pipeline::Builder &SetMode(CullMode cullMode, PolygonMode polygonMode, BlendMode blendMode);
                Pipeline::Builder &SetTransparency(bool enabled);
                Pipeline::Builder &SetSwapchainTarget(bool enabled);
                Pipeline::Builder &SetClearTargets(bool enabled);
                Pipeline::Builder &SetDepthOptions(bool depthTest, bool depthWrite);
                Pipeline::Builder &SetColorTarget(SharedPtr<Texture> &&pTexture, AttachmentType type);
                Pipeline::Builder &SetResolveTarget(SharedPtr<Texture> &pTexture);
                Pipeline::Builder &SetClearColor(float r, float g, float b, float a = 1.0f);
                Pipeline::Builder &SetLineWidth(float width);
                Pipeline::Builder &SetMipIndex(int index);
                Pipeline::Builder &SetSamples(uint8_t samples);
                Pipeline::Ptr Create(const SharedPtr<GraphicsContext> &pContext);
            };
            virtual ~Pipeline();

        public:
            const Properties &GetProperties() const { return *mProperties; }
            uint32_t GetWidth();
            uint32_t GetHeight();
            virtual void ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer) {}

        public:
            virtual void Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer = 0, uint32_t frame = 0) = 0;
            virtual void End(const SharedPtr<CommandBuffer> &commandBuffer) {}

        protected:
            Properties *mProperties;
            Pipeline(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
