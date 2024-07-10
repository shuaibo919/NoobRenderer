/* OpenGL RenderPass */
#include "Graphics/Backend/OpenGL/GLRenderContext.h"
/* Usage */

/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;

GLRenderContext::GLRenderContext()
{
}

GLRenderContext::~GLRenderContext()
{
}

void GLRenderContext::Init()
{
}

void GLRenderContext::Clear(uint32_t buffer)
{
    GlCall(glClear(GLUtilities::GetRendererBuffer(buffer)));
}

void GLRenderContext::Begin() {}
void GLRenderContext::OnResize(uint32_t width, uint32_t height) {}
void GLRenderContext::ClearRenderTarget(std::shared_ptr<Texture> texture, std::shared_ptr<CommandBuffer> commandBuffer, glm::vec4 clearColor) {}

void GLRenderContext::Present() {}
void GLRenderContext::Present(std::shared_ptr<CommandBuffer> commandBuffer) {}
void GLRenderContext::BindDescriptorSets(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<CommandBuffer> commandBuffer, uint32_t dynamicOffset, std::vector<std::shared_ptr<DescriptorSet>> descriptorSets) {}

const std::string &GLRenderContext::GetTitle() const { return ""; }
void GLRenderContext::DrawIndexed(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count, uint32_t start) const {}
void GLRenderContext::Draw(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count, DataType datayType, void *indices) const {}
void GLRenderContext::Dispatch(std::shared_ptr<CommandBuffer>, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) {}
void GLRenderContext::DrawSplashScreen(std::shared_ptr<Texture> texture) {}
uint32_t GLRenderContext::GetGPUCount() const
{
    return 1;
}
bool GLRenderContext::SupportsCompute()
{
    return false;
}
RHIFormat GLRenderContext::GetDepthFormat()
{
    return RHIFormat::Depth32Float;
}