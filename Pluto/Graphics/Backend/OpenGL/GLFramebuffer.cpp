/* OpenGL Framebuffer */
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLTexture.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLFramebuffer::GLFramebuffer(GLFramebuffer::Properties *&&pProperties)
    : Framebuffer(std::move(pProperties)), mColorAttachmentCount(0)
{

    if (pProperties->screenUse)
    {
        GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
    else
    {
        GlCall(glGenFramebuffers(1, &mHandle));
        GlCall(glBindFramebuffer(GL_FRAMEBUFFER, mHandle));
        for (auto i = 0; i < pProperties->attachments.size(); i++)
        {
            auto &texture = mProperties->attachments[i];
            switch (pProperties->attachmentTypes[i])
            {
            case AttachmentType::Color:
                AddTextureAttachment(texture, mProperties->mipIndex);
                break;
            case AttachmentType::Depth:
                AddTextureAttachment(texture);
                break;
            case AttachmentType::DepthArray:
                AddTextureLayer(texture, mProperties->layer);
                break;
            case AttachmentType::Cube:
                // AddCubeTextureAttachment(mProperties->)
                break;
            case AttachmentType::Other:
                break;
            }
        }
        GlCall(glDrawBuffers(static_cast<GLsizei>(mAttachmentChannels.size()), mAttachmentChannels.data()));

        Validate();

        UnBind();
    }
}

GLFramebuffer::~GLFramebuffer()
{
    if (!mProperties->screenUse)
        GlCall(glDeleteFramebuffers(1, &mHandle));
}

void GLFramebuffer::Bind() const
{
    GlCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mProperties->screenUse ? 0 : mHandle));
    GlCall(glViewport(0, 0, mProperties->width, mProperties->height));

    if (!mProperties->screenUse)
        GlCall(glDrawBuffers(static_cast<GLsizei>(mAttachmentChannels.size()), mAttachmentChannels.data()));
}
void GLFramebuffer::UnBind() const
{
    GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GLFramebuffer::AddTextureAttachment(std::shared_ptr<Texture> &texture, uint32_t mipLevel)
{
    GLenum attachment = GetAttachmentPoint(texture->GetProperties().format);
    GLuint handle = (GLuint)(size_t)(texture->GetHandle());

    if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT && attachment != GL_DEPTH_STENCIL_ATTACHMENT)
    {
        mAttachmentChannels.emplace_back(attachment);
    }
    GlCall(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, handle, mipLevel));
}
void GLFramebuffer::AddCubeTextureAttachment(CubeFace face, std::shared_ptr<TextureCube> &texture, uint32_t mipLevel)
{
    // uint32_t faceID = 0;
    // GLenum attachment = GetAttachmentPoint(texture->GetProperties().format);
    // GLuint handle = (GLuint)(texture->GetHandle());
    // if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT && attachment != GL_DEPTH_STENCIL_ATTACHMENT)
    // {
    //     mAttachmentChannels.emplace_back(attachment);
    // }

    // switch (face)
    // {
    // case CubeFace::PositiveX:
    //     faceID = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    //     break;
    // case CubeFace::NegativeX:
    //     faceID = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    //     break;
    // case CubeFace::PositiveY:
    //     faceID = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    //     break;
    // case CubeFace::NegativeY:
    //     faceID = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    //     break;
    // case CubeFace::PositiveZ:
    //     faceID = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    //     break;
    // case CubeFace::NegativeZ:
    //     faceID = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    //     break;
    // }

    // GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, faceID, handle, mipLevel));
}
void GLFramebuffer::AddDepthAttachment(std::shared_ptr<Texture> &texture)
{
    GLuint handle = GLUtilities::PtrToGLuint(texture->GetHandle());
    GlCall(glFramebufferTextureLayer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, handle, 0, 0));
    GlCall(glDrawBuffers(0, GL_NONE));
}

void GLFramebuffer::AddTextureLayer(std::shared_ptr<Texture> &texture, int index)
{
    GLuint handle = GLUtilities::PtrToGLuint(texture->GetHandle());
    GlCall(glFramebufferTextureLayer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, (GLuint)(size_t)texture->GetHandle(), 0, index));
}

void GLFramebuffer::Validate()
{
    uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        log<Critical>("Unable to create Framebuffer! StatusCode: %d", status);
    }
}

void GLFramebuffer::SetClearColor(const glm::vec4 &color)
{
    mClearColor = color;
}

GLenum GLFramebuffer::GetAttachmentPoint(RHIFormat format)
{
    if (Utilities::IsDepthStencilFormat(format))
    {
        return GL_DEPTH_STENCIL_ATTACHMENT;
    }

    if (Utilities::IsStencilFormat(format))
    {
        return GL_STENCIL_ATTACHMENT;
    }

    if (Utilities::IsDepthFormat(format))
    {
        return GL_DEPTH_ATTACHMENT;
    }

    GLenum value = GL_COLOR_ATTACHMENT0 + mColorAttachmentCount;
    mColorAttachmentCount++;
    return value;
}