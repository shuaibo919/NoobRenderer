#include "Platform.h"
#include "Graphics/RHI/Pipeline.h"
#include "Graphics/RHI/Texture.h"
#include "Graphics/RHI/RenderDevice.h"
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/RHI/RenderCommand.h"
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
    auto ctx = GraphicsContext::Create(PLATFORM_API);
    auto window = Window::Create(ctx, 600, 600, "Test");
    ctx->Init();
    ctx->BindToDevice();
    ctx->SetMainSwapChain(window->GetSwapChainProperties());
    auto pVertexBuffer = VertexBuffer::Builder()
                             .SetVertexData(vertices, 3, sizeof(vertices))
                             .SetUsage(BufferUsage::Static)
                             .SetAttribute(VertexAttributeType::Position, 0, ElementType::Float3, 0, 3 * sizeof(float))
                             .Create(ctx);

    auto pShader = Shader::Builder()
                       .SetFile("Asset/Shader/TestTriangle.shader.json")
                       .Create(ctx);

    auto pPipeline = Pipeline::Builder()
                         .SetClearColor(0.2f, 0.2f, 0.2f, 1.0f)
                         .SetDepthOptions(false, false)
                         .SetShader(pShader)
                         .SetDrawType(DrawType::Triangle)
                         .SetSwapchainTarget(true)
                         .SetClearTargets(true)
                         .Create(ctx);

    auto pDescriptorSet = DescriptorSet::Builder()
                              .SetBindingLayout(pShader, 0)
                              .Create(ctx);

    auto pContext = ctx->GetRenderContext();

    auto pRenderCommand = RenderCommand::Builder()
                              .SetUsageType(CommandBufferUsageType::RecycleSubmit)
                              .Create(ctx);

    UniformDataMat4 model;
    model.name = "model";
    model.blockname = "UniformBufferObject";
    model.data = glm::mat4(1.0f);

    UniformDataMat4 view;
    view.name = "view";
    view.blockname = "UniformBufferObject";
    view.data = glm::mat4(1.0f);

    UniformDataMat4 projection;
    projection.name = "proj";
    projection.blockname = "UniformBufferObject";
    projection.data = glm::mat4(1.0f);

    pDescriptorSet->SetUniform(projection.blockname, projection.name, &projection.data);
    pDescriptorSet->SetUniform(model.blockname, model.name, &model.data);
    pDescriptorSet->SetUniform(view.blockname, view.name, &view.data);
    pDescriptorSet->Update();

    {
        pRenderCommand->BeginRecording();
        pRenderCommand->BindPipeline(pPipeline);
        pRenderCommand->BindDescriptorSet(pPipeline, 0, pDescriptorSet);
        pRenderCommand->BindVetexBuffer(pPipeline, pVertexBuffer);
        pRenderCommand->Draw(DrawType::Triangle, 3);
        pRenderCommand->UnBindPipeline();
        pRenderCommand->EndRecording();
    }

    while (!window->ShouldClose())
    {
        pContext->GetSwapChain()->BeginFrame();
        {
            pContext->GetSwapChain()->Submit(pRenderCommand);
        }
        pContext->GetSwapChain()->EndFrame();
        window->PollEvents();
        window->SwapBuffers();
    }
    window->Terminate();
    ctx->Terminate();
    return 0;
}
