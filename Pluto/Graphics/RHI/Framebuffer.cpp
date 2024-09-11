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

Framebuffer::Builder &Framebuffer::Builder::SetRenderPass(SharedPtr<RenderPass> &&renderPass)
{
    mProperties->renderPass = std::forward<SharedPtr<RenderPass>>(renderPass);
    return *this;
}

Framebuffer::Builder &Framebuffer::Builder::SetAttachment(SharedPtr<Texture> &&texture, AttachmentType type)
{
    mProperties->attachments.push_back(std::forward<SharedPtr<Texture>>(texture));
    mProperties->attachmentTypes.push_back(type);
    return *this;
}

Framebuffer::Ptr Framebuffer::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
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
