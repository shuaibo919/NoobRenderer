/* OpenGL VertexBuffer */
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLUniformBuffer.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLDescriptorSet::GLDescriptorSet(RenderContext *ctx, GLDescriptorSet::Properties *&&pProperties)
    : DescriptorSet(ctx, std::move(pProperties))
{
}

GLDescriptorSet::~GLDescriptorSet()
{
}

// void GLDescriptorSet::Update(std::shared_ptr<CommandBuffer> buffer)
// {
//     for (auto &bufferInfo : mUniformBuffers)
//     {
//         if (bufferInfo.second.updated)
//         {
//             // bufferInfo.second.ubo->SetData(bufferInfo.second.LocalStorage.Data);
//             // bufferInfo.second.HasUpdated = false;
//         }
//     }
// }
