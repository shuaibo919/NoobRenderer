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
    auto renderSystem = GraphicsContext::Create(RenderAPI::OPENGL, RenderDevice::Get());
    auto window = Window::Create(renderSystem, 300, 100, "Test");
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
                            .Create(renderSystem);

    auto cmdBuffer = CommandBuffer::Builder()
                         .Create(renderSystem);

    // auto shader = Shader::Builder()
    //                   .SetFile("Asset/Shader/Test.shader.json")
    //                   .Create(renderSystem);

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