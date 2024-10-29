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

using namespace pluto;
using namespace pluto::Graphics;

Shader::Ptr pShader{nullptr};
Pipeline::Ptr pPipeline{nullptr};
Texture::Ptr pTexture{nullptr};
VertexBuffer::Ptr pVertexBuffer{nullptr};
DescriptorSet::Ptr pDescriptorSet{nullptr};
RenderCommand::Ptr pRenderCommand{nullptr};

void PrepareRenderData(const SharedPtr<pluto::Graphics::GraphicsContext> &context);

int main()
{
    using namespace pluto;
    using namespace pluto::Graphics;
    RenderDevice::Init();
    auto ctx = GraphicsContext::Create(PLATFORM_API);
    auto window = Window::Create(ctx, 600, 600, "Test_01_Triangle");
    ctx->Init();
    ctx->BindToDevice();
    ctx->SetMainSwapChain(window->GetSwapChainProperties());

    PrepareRenderData(ctx);

    // Recording Command
    pRenderCommand->BeginRecording();
    pRenderCommand->BindPipeline(pPipeline);
    pRenderCommand->BindDescriptorSet(pPipeline, 0, pDescriptorSet);
    pRenderCommand->BindVetexBuffer(pPipeline, pVertexBuffer);
    pRenderCommand->Draw(DrawType::Triangle, 3);
    pRenderCommand->UnBindPipeline();
    pRenderCommand->EndRecording();

    auto pContext = ctx->GetRenderContext();

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

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f};

struct UniformDataMat4
{
    std::string name;
    std::string blockname;
    glm::mat4 data;
};

UniformDataMat4 model, view, projection;

void PrepareRenderData(const SharedPtr<pluto::Graphics::GraphicsContext> &context)
{
    pVertexBuffer = VertexBuffer::Builder()
                        .SetVertexData(vertices, 3, sizeof(vertices))
                        .SetUsage(BufferUsage::Static)
                        .SetAttribute(VertexAttributeType::Position, 0, ElementType::Float3, 0, 3 * sizeof(float))
                        .Create(context);

    pShader = Shader::Builder()
                  .SetFile("Asset/Shader/TestTexture.shader.json")
                  .Create(context);

    pTexture = Texture::Builder()
                   .SetWrap(TextureWrap::ClampToedge)
                   .SetHints(Texture::Hints::ShaderUse)
                   .Create(Texture::Type::Texture2D,
                           "Asset/Texture/wood_seamless_texture.png", context);

    pPipeline = Pipeline::Builder()
                    .SetClearColor(0.2f, 0.2f, 0.2f, 1.0f)
                    .SetDepthOptions(false, false)
                    .SetShader(pShader)
                    .SetDrawType(DrawType::Triangle)
                    .SetSwapchainTarget(true)
                    .SetClearTargets(true)
                    .Create(context);

    pDescriptorSet = DescriptorSet::Builder()
                         .SetBindingLayout(pShader, 0)
                         .Create(context);

    pRenderCommand = RenderCommand::Builder()
                         .SetUsageType(CommandBufferUsageType::RecycleSubmit)
                         .Create(context);

    model.name = "model";
    model.blockname = "UniformBufferObject";
    model.data = glm::mat4(1.0f);

    view.name = "view";
    view.blockname = "UniformBufferObject";
    view.data = glm::mat4(1.0f);

    projection.name = "proj";
    projection.blockname = "UniformBufferObject";
    projection.data = glm::mat4(1.0f);

    pDescriptorSet->SetUniform(projection.blockname, projection.name, &projection.data);
    pDescriptorSet->SetUniform(model.blockname, model.name, &model.data);
    pDescriptorSet->SetUniform(view.blockname, view.name, &view.data);
    pDescriptorSet->SetTexture("texSampler", pTexture);
    pDescriptorSet->Update();
}