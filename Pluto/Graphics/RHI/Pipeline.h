#pragma once
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class GraphicsContext;
        class Pipeline
        {
            friend class CommandBuffer;
            friend class GLCommandBuffer;
            friend class VKCommandBuffer;
            friend class GraphicsContext;

        public:
            using Ptr = std::shared_ptr<Pipeline>;
            const static uint8_t MaxRenderTargets = 8;
            struct Properties
            {
                std::shared_ptr<Shader> shader;

                CullMode cullMode = CullMode::BACK;
                PolygonMode polygonMode = PolygonMode::FILL;
                DrawType drawType = DrawType::TRIANGLE;
                BlendMode blendMode = BlendMode::None;

                bool transparencyEnabled{false};
                bool swapchainTarget{false};
                bool clearTargets{false};

                bool DepthTest{true};
                bool DepthWrite{true};

                std::array<std::shared_ptr<Texture>, Pipeline::MaxRenderTargets> colourTargets{};
                std::shared_ptr<Texture> resolveTexture = nullptr;
                float clearColor[4]{0.2f, 0.2f, 0.2f, 1.0f};
                float lineWidth{1.0f};
                int mipIndex{0};
                uint8_t samples{1};
            };
            struct Builder final : BuildBase<Pipeline::Properties, Pipeline>
            {
                Builder() {}
                Pipeline::Builder &SetShader(std::shared_ptr<Shader> &pShader);
                Pipeline::Builder &SetDrawType(DrawType drawType);
                Pipeline::Builder &SetMode(CullMode cullMode, PolygonMode polygonMode, BlendMode blendMode);
                Pipeline::Builder &SetTransparency(bool enabled);
                Pipeline::Builder &SetSwapchainTarget(bool enabled);
                Pipeline::Builder &SetClearTargets(bool enabled);
                Pipeline::Builder &SetDepthOptions(bool depthTest, bool depthWrite);
                Pipeline::Builder &SetColourTarget(std::shared_ptr<Texture> &pTexture);
                Pipeline::Builder &SetResolveTarget(std::shared_ptr<Texture> &pTexture);
                Pipeline::Builder &SetClearColor(float r, float g, float b, float a = 1.0f);
                Pipeline::Builder &SetLineWidth(float width);
                Pipeline::Builder &SetMipIndex(int index);
                Pipeline::Builder &SetSamples(uint8_t samples);
                Pipeline::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~Pipeline();

        public:
            const Properties &GetProperties() const { return *mProperties; }
            static void ClearCache();
            static void DeleteUnusedCache();

            virtual void ClearRenderTargets(std::shared_ptr<CommandBuffer> commandBuffer) {}

        protected:
            virtual void Bind(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t layer = 0) = 0;
            virtual void End(std::shared_ptr<CommandBuffer> commandBuffer) {}

        protected:
            Properties *mProperties;
            Pipeline(Properties *&&pProperties);
        };
    }
}
