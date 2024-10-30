/* OpenGL Render Command */
#include "Graphics/Backend/OpenGL/GLRenderCommand.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLCommandEmulated.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLPipeline.h"
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"
#include "GLRenderCommand.h"
using namespace pluto;
using namespace pluto::Graphics;

GLRenderCommand::GLRenderCommand(RenderContext *ctx, Properties *&&pProperties)
    : RenderCommand(ctx, std::move(pProperties))
{
    NRE_ASSERT(false,"NOT IMPLEMENTED!");
}

GLRenderCommand::~GLRenderCommand()
{
}

void GLRenderCommand::Reset()
{
}

bool GLRenderCommand::Flush()
{
    return true;
}

void GLRenderCommand::BeginRecording()
{
}

void GLRenderCommand::EndRecording()
{
}

void GLRenderCommand::BindVetexBuffer(const SharedPtr<Pipeline> &pipeline, const SharedPtr<VertexBuffer> &vbo, uint8_t binding)
{
}

void GLRenderCommand::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
}

void GLRenderCommand::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
}

void GLRenderCommand::UpdateViewport(uint32_t width, uint32_t height, bool flipViewport)
{
}

void GLRenderCommand::BindPipeline(const SharedPtr<Pipeline> &pipeline)
{
}

void GLRenderCommand::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
{
}

void GLRenderCommand::DrawIndexed(DrawType type, uint32_t count, uint32_t start)
{
}

void GLRenderCommand::Draw(DrawType type, uint32_t count)
{
}

void GLRenderCommand::Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ)
{
}

void GLRenderCommand::DrawSplashScreen(const SharedPtr<Texture> &texture)
{
}

void GLRenderCommand::UnBindPipeline()
{
}
