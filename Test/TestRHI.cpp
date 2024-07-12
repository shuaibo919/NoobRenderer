#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/RenderDevice.h"
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/RHI/CommandBuffer.h"
#include "Graphics/RHI/Pipeline.h"
#include "Graphics/RHI/Texture.h"
#include "Graphics/RHI/Shader.h"
#include "Graphics/Window.h"
#include "Graphics/Backend/OpenGL/GL.h"
#include "Core/Log.hpp"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

int main()
{
    using namespace pluto;
    using namespace pluto::Graphics;
    RenderDevice::Init();
    RenderDevice::Create();
    auto ctx = GraphicsContext::Create(RenderAPI::OPENGL, RenderDevice::Get());
    auto window = Window::Create(ctx, 300, 100, "Test");
    ctx->SetMainSwapChain(window->GetSwapChain());
    // test logger
    if (window != nullptr)
    {
        log("Window Created");
        log<Info>("Window Created");
        log<Debug>("Window Created");
    }

    auto vertexBuffer = VertexBuffer::Builder()
                            .SetVertexData(vertices, 3)
                            .SetUsage(BufferUsage::Static)
                            .SetAttribute(VertexAttributeType::Position, 0, ElementType::Float3, 3, 0)
                            .Create(ctx);

    auto cmdBuffer = CommandBuffer::Builder()
                         .Create(ctx);

    auto shader = Shader::Builder()
                      .SetFile("Asset/Shader/Test.shader.json")
                      .Create(ctx);

    auto colorTarget = Texture::Builder()
                           .SetBase(300, 100, 1, RHIFormat::R16G16B16A16Float)
                           .SetFilter(TextureFilter::Linear, TextureFilter::Linear)
                           .SetWrap(TextureWrap::ClampToedge)
                           .Create(Texture::Type::Texture2D, ctx);

    auto pipeline = Pipeline::Builder()
                        .SetClearColor(1.0f, 0.3f, 0.3f, 1.0f)
                        .SetShader(shader)
                        .SetDepthOptions(false, false)
                        .SetDrawType(DrawType::Triangle)
                        .SetSwapchainTarget(true)
                        .SetColorTarget(std::move(colorTarget), AttachmentType::Color)
                        .SetClearTargets(true)
                        .Create(ctx);

    while (!window->ShouldClose())
    {
        window->PollEvents();
        {
        }

        window->SwapBuffers();
    }
    window->Terminate();
    RenderDevice::Release();

    return 0;
}