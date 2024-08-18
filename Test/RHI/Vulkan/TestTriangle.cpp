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
    auto window = Window::Create(ctx, 300, 100, "Test");
    ctx->Init();
    ctx->BindToDevice();
    ctx->SetMainSwapChain(window->GetSwapChainProperties());

    VertexBuffer::Ptr vertexBuffer = VertexBuffer::Builder()
                                         .SetVertexData(vertices, 3, sizeof(vertices))
                                         .SetUsage(BufferUsage::Static)
                                         .SetAttribute(VertexAttributeType::Position, 0, ElementType::Float3, 0, 3 * sizeof(float))
                                         .Create(ctx);

    Shader::Ptr shader = Shader::Builder()
                             .SetFile("Asset/Shader/TestTriangle.shader.json")
                             .Create(ctx);

    Texture2D::Ptr colorTarget = Texture::Builder()
                                     .SetBase(300, 100, 1, RHIFormat::R16G16B16A16Float)
                                     .SetFilter(TextureFilter::Linear, TextureFilter::Linear)
                                     .SetWrap(TextureWrap::ClampToedge)
                                     .Create(Texture::Type::Texture2D, ctx);

    CommandBuffer::Ptr cmdBuffer = CommandBuffer::Builder()
                                       .SetUsageType(CommandBufferUsageType::RecycleSubmit)
                                       .Create(ctx);

    Pipeline::Ptr pipeline = Pipeline::Builder()
                                 .SetClearColor(0.2f, 0.2f, 0.2f, 1.0f)
                                 .SetDepthOptions(false, false)
                                 .SetShader(shader)
                                 .SetDrawType(DrawType::Triangle)
                                 .SetSwapchainTarget(true)
                                 .SetColorTarget(std::move(colorTarget), AttachmentType::Color)
                                 .SetClearTargets(true)
                                 .Create(ctx);
    auto descriptorSet = DescriptorSet::Builder()
                             .SetBindingLayout(shader, 0)
                             .Create(ctx);

    // UniformDataMat4 model;
    // model.name = "model";
    // model.blockname = "UniformBufferObject";
    // model.data = glm::mat4(1.0f);

    // UniformDataMat4 view;
    // view.name = "view";
    // view.blockname = "UniformBufferObject";
    // view.data = glm::mat4(1.0f);

    // UniformDataMat4 projection;
    // projection.name = "proj";
    // projection.blockname = "UniformBufferObject";
    // projection.data = glm::mat4(1.0f);

    // descriptorSet->SetUniform(projection.blockname, projection.name, &projection.data);
    // descriptorSet->SetUniform(model.blockname, model.name, &model.data);
    // descriptorSet->SetUniform(view.blockname, view.name, &view.data);
    // descriptorSet->Update();

    while (!window->ShouldClose())
    {
        {
        }
        window->PollEvents();
        window->SwapBuffers();
    }
    window->Terminate();
    ctx->Terminate();

    return 0;
}
