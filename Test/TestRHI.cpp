#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/RenderDevice.h"
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/RHI/Texture.h"
#include "Graphics/Window.h"
#include "Graphics/Backend/OpenGL/GL.h"
#include "Core/Log.hpp"
int main()
{
    using namespace pluto;
    using namespace pluto::Graphics;
    RenderDevice::Init();
    RenderDevice::Create();
    auto renderSystem = GraphicsContext::Create(RenderAPI::OPENGL, RenderDevice::Get());
    auto window = Window::Create(renderSystem, 300, 100, "Test");
    // test logger
    if (window != nullptr)
    {
        log("Window Created");
        log<Info>("Window Created");
        log<Debug>("Window Created");
    }
    // test:vertexBuffer creation
    auto vertexBuffer = VertexBuffer::Builder()
                            .SetVertexData(nullptr, 0)
                            .SetUsage(BufferUsage::Static)
                            .SetAttribute(VertexAttributeType::Position, 0, ElementType::Float3, 3, 0)
                            .Create(renderSystem);

    // test:Texture creation, way 1
    auto texture1 = Texture::Builder()
                        .SetBase(10, 10, 1, RHIFormat::R16G16B16A16_Float)
                        .SetFilter(TextureFilter::Linear, TextureFilter::Linear)
                        .SetWrap(TextureWrap::REPEAT)
                        .Create(Texture::Type::Texture2D, renderSystem);

    while (!window->ShouldClose())
    {
        window->PollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->SwapBuffers();
    }
    window->Terminate();
    RenderDevice::Release();

    return 0;
}