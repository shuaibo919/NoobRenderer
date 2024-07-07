#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLVertexBuffer::GLVertexBuffer(VertexBuffer::Properties *&&pProperties)
    : VertexBuffer(std::move(pProperties))
{
    DelayedBackendInitialize();
    glGenBuffers(1, &mHandle);
    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
    glBufferData(GL_ARRAY_BUFFER, mProperties->count, mProperties->data, GLUtilities::GetBufferUsage(mProperties->usage));
    uint16_t attriArrayPosition = 0;
    for (size_t i = 0; i < mProperties->usedAttributes.size(); ++i)
    {
        if (mProperties->usedAttributes[i])
        {
            GLuint index = static_cast<GLuint>(i);
            GLint count = GLUtilities::GetElementCount(mProperties->attributes[i].type);
            GLenum type = GLUtilities::GetDataType(mProperties->attributes[i].type);
            GLsizei stride = mProperties->attributes[i].stride;
            void const *pointer = reinterpret_cast<void const *>(mProperties->attributes[i].offset);

            glEnableVertexAttribArray(attriArrayPosition);
            glVertexAttribPointer(attriArrayPosition, count, type, GL_FALSE, stride, pointer);
            attriArrayPosition++;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &mHandle);
}

void GLVertexBuffer::DelayedBackendInitialize()
{
}

void GLVertexBuffer::Bind(CommandBuffer *commandBuffer, Pipeline *pipeline, uint8_t binding)
{
}

void GLVertexBuffer::Unbind()
{
}
// #include "Precompiled.h"
// #include "GLVertexBuffer.h"
// #include "GLPipeline.h"
// #include "GLRenderer.h"

// #include "GL.h"

// namespace Lumos
// {
//     namespace Graphics
//     {

//         GLVertexBuffer::GLVertexBuffer(BufferUsage usage)
//             : m_Usage(usage)
//             , m_Size(0)
//         {
//             LUMOS_PROFILE_FUNCTION();
//             GLCall(glGenBuffers(1, &m_Handle));
//         }

//         GLVertexBuffer::~GLVertexBuffer()
//         {
//             LUMOS_PROFILE_FUNCTION();
//             GLCall(glDeleteBuffers(1, &m_Handle));
//         }

//         void GLVertexBuffer::Resize(uint32_t size)
//         {
//             LUMOS_PROFILE_FUNCTION();
//             m_Size = size;

//             GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
//             GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, BufferUsageToOpenGL(m_Usage)));
//         }

//         void GLVertexBuffer::SetData(uint32_t size, const void* data, bool addBarrier)
//         {
//             LUMOS_PROFILE_FUNCTION();
//             LUMOS_UNUSED(addBarrier);
//             m_Size = size;
//             GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
//             GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageToOpenGL(m_Usage)));
//         }

//         void GLVertexBuffer::SetDataSub(uint32_t size, const void* data, uint32_t offset)
//         {
//             LUMOS_PROFILE_FUNCTION();
//             m_Size = size;
//             GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
//             GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
//         }

//         void* GLVertexBuffer::GetPointerInternal()
//         {
//             LUMOS_PROFILE_FUNCTION();
//             void* result = nullptr;
//             if(!m_Mapped)
//             {
//                 GLCall(result = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
//                 m_Mapped = true;
//             }
//             else
//             {
//                 LUMOS_LOG_WARN("Vertex buffer already mapped");
//             }

//             return result;
//         }

//         void GLVertexBuffer::ReleasePointer()
//         {
//             LUMOS_PROFILE_FUNCTION();
//             if(m_Mapped)
//             {
//                 GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
//                 m_Mapped = false;
//             }
//         }

//         void GLVertexBuffer::Bind(CommandBuffer* commandBuffer, Pipeline* pipeline, uint8_t binding)
//         {
//             LUMOS_PROFILE_FUNCTION();
//             GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
//             GLRenderer::Instance()->GetBoundVertexBuffer() = m_Handle;

//             if(pipeline)
//                 ((GLPipeline*)pipeline)->BindVertexArray();
//         }

//         void GLVertexBuffer::Unbind()
//         {
//             LUMOS_PROFILE_FUNCTION();
//             GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//             GLRenderer::Instance()->GetBoundVertexBuffer() = -1;
//         }

//         void GLVertexBuffer::Init()
//         {
//             CreateFunc         = CreateFuncGL;
//             CreateWithDataFunc = CreateFuncWithDataGL;
//         }

//         VertexBuffer* GLVertexBuffer::CreateFuncGL(const BufferUsage& usage)
//         {
//             return new GLVertexBuffer(usage);
//         }

//         VertexBuffer* GLVertexBuffer::CreateFuncWithDataGL(uint32_t size, const void* data, const BufferUsage& usage)
//         {
//             GLVertexBuffer* buffer = new GLVertexBuffer(usage);
//             buffer->SetData(size, data);
//             return buffer;
//         }
//     }
// }
