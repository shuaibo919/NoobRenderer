#include "Graphics/RHI/Pipeline.h"
#include "Graphics/RHI/Texture.h"
#include "Graphics/RHI/RenderDevice.h"
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/RHI/CommandBuffer.h"
#include "Graphics/RHI/DescriptorSet.h"
#include "Graphics/RHI/RenderContext.h"
#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/Shader.h"
#include "Graphics/Window.h"
#include "Core/Log.hpp"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

struct UniformDataMat4
{
    std::string name;
    std::string blockname;
    glm::mat4 data;
};

int main()
{
    using namespace pluto;
    using namespace pluto::Graphics;
    RenderDevice::Init();
    auto ctx = GraphicsContext::Create(RenderAPI::VULKAN);
    auto window = Window::Create(ctx, 600, 600, "Test");
    ctx->Init();
    ctx->BindToDevice();
    ctx->SetMainSwapChain(window->GetSwapChain());

    while (!window->ShouldClose())
    {
        {
        }
        window->PollEvents();
        window->SwapBuffers();
    }
    window->Terminate();

    return 0;
}
