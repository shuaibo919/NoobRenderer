#include "Platform.h"
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
    auto ctx = GraphicsContext::Create(PLATFORM_API);
    auto window = Window::Create(ctx, 600, 600, "Test");
    ctx->Init();
    ctx->BindToDevice();
    ctx->SetMainSwapChain(window->GetSwapChainProperties());
    auto vertexBuffer = VertexBuffer::Builder()
                            .SetVertexData(vertices, 3, sizeof(vertices))
                            .SetUsage(BufferUsage::Static)
                            .SetAttribute(VertexAttributeType::Position, 0, ElementType::Float3, 0, 3 * sizeof(float))
                            .Create(ctx);

    auto shader = Shader::Builder()
                      .SetFile("Asset/Shader/TestTriangle.shader.json")
                      .Create(ctx);

    auto pipeline = Pipeline::Builder()
                        .SetClearColor(0.2f, 0.2f, 0.2f, 1.0f)
                        .SetDepthOptions(false, false)
                        .SetShader(shader)
                        .SetDrawType(DrawType::Triangle)
                        .SetSwapchainTarget(true)
                        .SetClearTargets(true)
                        .Create(ctx);

    auto descriptorSet = DescriptorSet::Builder()
                             .SetBindingLayout(shader, 0)
                             .Create(ctx);

    auto context = ctx->GetRenderContext();

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

    descriptorSet->SetUniform(projection.blockname, projection.name, &projection.data);
    descriptorSet->SetUniform(model.blockname, model.name, &model.data);
    descriptorSet->SetUniform(view.blockname, view.name, &view.data);

    while (!window->ShouldClose())
    {
        context->GetSwapChain()->BeginFrame();
        {
            auto pFrameCommandBuffer = context->GetSwapChain()->GetCurrentCommandBuffer();
            pFrameCommandBuffer->BeginRecording();
            pFrameCommandBuffer->BindPipeline(pipeline);
            pFrameCommandBuffer->BindDescriptorSet(pipeline, 0, descriptorSet);
            descriptorSet->Update(pFrameCommandBuffer);
            pFrameCommandBuffer->BindVetexBuffer(pipeline, vertexBuffer);
            pFrameCommandBuffer->Draw(DrawType::Triangle, 3);
            pFrameCommandBuffer->UnBindPipeline();
            pFrameCommandBuffer->EndRecording();

            context->GetSwapChain()->Submit(pFrameCommandBuffer);
        }
        context->GetSwapChain()->EndFrame();
        window->PollEvents();
        window->SwapBuffers();
    }
    window->Terminate();
    ctx->Terminate();
    return 0;
}
