#include "Graphics/RHI/Framebuffer.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;

Framebuffer::Builder &Framebuffer::Builder::SetBase(uint32_t width, uint32_t height, uint32_t layer)
{
    mProperties->width = width;
    mProperties->height = height;
    mProperties->layer = layer;
    return *this;
}

Framebuffer::Builder &Framebuffer::Builder::SetMipIndex(int index)
{
    mProperties->mipIndex = index;
    return *this;
}

Framebuffer::Builder &Framebuffer::Builder::SetSamples(uint32_t samples)
{
    mProperties->samples = samples;
    return *this;
}

Framebuffer::Builder &Framebuffer::Builder::SetUseScreen(bool use)
{
    mProperties->screenUse = use;
    return *this;
}

Framebuffer::Builder &Framebuffer::Builder::SetRenderPass(std::shared_ptr<RenderPass> &&renderPass)
{
    mProperties->renderPass = std::forward<std::shared_ptr<RenderPass>>(renderPass);
    return *this;
}

Framebuffer::Builder &Framebuffer::Builder::SetAttachment(std::shared_ptr<Texture> &&texture, AttachmentType type)
{
    mProperties->attachments.push_back(std::forward<std::shared_ptr<Texture>>(texture));
    mProperties->attachmentTypes.push_back(type);
    return *this;
}

Framebuffer::Ptr Framebuffer::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateFrameBuffer(std::move(mProperties));
}

Framebuffer::Framebuffer(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

Framebuffer::~Framebuffer()
{
    if (mProperties != nullptr)
        delete mProperties;
}
