#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/RenderContext.h"
#include "Graphics/RHI/RenderDevice.h"
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/RHI/CommandBuffer.h"
#include "Graphics/RHI/DescriptorSet.h"
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

    auto descriptorSet = DescriptorSet::Builder()
                             .SetBindingLayout(shader, 0)
                             .Create(ctx);

    auto rctx = ctx->GetRenderContext();

    UniformDataMat4 model;
    model.name = "model";
    model.blockname = "UniformBufferObject";
    model.data = glm::mat4(1.0f);

    UniformDataMat4 view;
    view.name = "model";
    view.blockname = "UniformBufferObject";
    view.data = glm::mat4(1.0f);

    UniformDataMat4 projection;
    projection.name = "model";
    projection.blockname = "UniformBufferObject";
    projection.data = glm::mat4(1.0f);
    descriptorSet->SetUniform(model.name, model.blockname, glm::value_ptr<float>(model.data));
    descriptorSet->SetUniform(view.name, view.blockname, glm::value_ptr<float>(view.data));
    descriptorSet->SetUniform(projection.name, projection.blockname, glm::value_ptr<float>(projection.data));

    while (!window->ShouldClose())
    {
        window->PollEvents();
        {
            cmdBuffer->BeginRecording();
            cmdBuffer->BindPipeline(pipeline);
            pipeline->ClearRenderTargets(cmdBuffer);
            rctx->BindDescriptorSet(pipeline, cmdBuffer, 0, descriptorSet);
            pipeline->End(cmdBuffer);

            cmdBuffer->EndRecording();
        }

        window->SwapBuffers();
    }
    window->Terminate();
    RenderDevice::Release();

    return 0;
}