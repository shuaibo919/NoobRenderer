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

namespace NativeGLDrawWrapper
{
    void DrawArrays(GLenum mode, GLint first, GLsizei count)
    {
        GlCall(glDrawArrays(mode, first, count));
    }

    void DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
    {
        GlCall(glDrawElements(mode, count, type, indices));
    }

    void DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instanceCount)
    {
        GlCall(glDrawElementsInstanced(mode, count, type, indices, instanceCount));
    }

    void DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount)
    {
        GlCall(glDrawArraysInstanced(mode, first, count, instanceCount));
    }
}

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
    mRenderOperations.emplace_back(std::bind(&GLVertexBuffer::Bind, static_cast<GLVertexBuffer*>(vbo.get()), nullptr, pipeline, binding));
}

void GLRenderCommand::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
    mRenderOperations.emplace_back(std::bind(&GLDescriptorSet::Bind, static_cast<GLDescriptorSet*>(descriptorSet.get()), nullptr, pipeline, dynamicOffset));
}

void GLRenderCommand::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
    mRenderOperations.emplace_back(std::bind(&GLDescriptorSet::Bind, static_cast<GLDescriptorSet*>(descriptorSets[0].get()), nullptr, pipeline, dynamicOffset));
}

void GLRenderCommand::UpdateViewport(uint32_t width, uint32_t height, bool flipViewport)
{
}

void GLRenderCommand::BindPipeline(const SharedPtr<Pipeline> &pipeline)
{
    mRenderOperations.emplace_back(std::bind(&GLPipeline::Bind, static_cast<GLPipeline*>(pipeline.get()), nullptr));
}

void GLRenderCommand::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
{
    mRenderOperations.emplace_back(std::bind(&GLPipeline::Bind, static_cast<GLPipeline*>(pipeline.get()), nullptr, layer));
}

void GLRenderCommand::DrawIndexed(DrawType type, uint32_t count, uint32_t start)
{
    // mRenderOperations.emplace_back(std::bind(&GLCommandEmulated::DrawIndexed, nullptr, type, count, start));
}

void GLRenderCommand::Draw(DrawType type, uint32_t count)
{
    mRenderOperations.emplace_back(std::bind(&NativeGLDrawWrapper::DrawArrays, GLUtilities::GetDrawType(type), 0, count));
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
