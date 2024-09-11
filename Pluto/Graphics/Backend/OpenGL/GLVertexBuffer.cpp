/* OpenGL VertexBuffer */
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLRenderContext.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLPipeline.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLVertexBuffer::GLVertexBuffer(RenderContext *ctx, VertexBuffer::Properties *&&pProperties)
    : VertexBuffer(ctx, std::move(pProperties))
{
    GlCall(glGenBuffers(1, &mHandle));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, mHandle));
    GlCall(glBufferData(GL_ARRAY_BUFFER, mProperties->vetexSize, mProperties->data, GLUtilities::GetBufferUsage(mProperties->usage)));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

GLVertexBuffer::~GLVertexBuffer()
{
    GlCall(glDeleteBuffers(1, &mHandle));
}

void GLVertexBuffer::Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding)
{
    GLRenderContext::CurrentVertexHandle.handle = mHandle;
    GLRenderContext::CurrentVertexHandle.valid = true;
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, mHandle));

    if (pipeline != nullptr)
        std::dynamic_pointer_cast<GLPipeline>(pipeline)->BindVertexArray(this->Get());
}

void GLVertexBuffer::Unbind()
{
    GLRenderContext::CurrentVertexHandle.handle = 0;
    GLRenderContext::CurrentVertexHandle.valid = false;
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}