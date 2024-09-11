#include "Graphics/RHI/Pipeline.h"
#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/Texture.h"
#include "Core/Utilities.h"

using namespace pluto::Graphics;

Pipeline::Builder &Pipeline::Builder::SetShader(const SharedPtr<Shader> &pShader)
{
    mProperties->shader = pShader;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetDrawType(DrawType drawType)
{
    mProperties->drawType = drawType;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetMode(CullMode cullMode, PolygonMode polygonMode, BlendMode blendMode)
{
    mProperties->cullMode = cullMode;
    mProperties->polygonMode = polygonMode;
    mProperties->blendMode = blendMode;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetTransparency(bool enabled)
{
    mProperties->transparencyEnabled = enabled;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetSwapchainTarget(bool enabled)
{
    mProperties->swapchainTarget = enabled;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetClearTargets(bool enabled)
{
    mProperties->clearTargets = enabled;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetDepthOptions(bool depthTest, bool depthWrite)
{
    mProperties->DepthTest = depthTest;
    mProperties->DepthWrite = depthWrite;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetColorTarget(SharedPtr<Texture> &&pTexture, AttachmentType type)
{
    mProperties->colorTargets.push_back(std::forward<SharedPtr<Texture>>(pTexture));
    mProperties->attachmentTypes.push_back(type);
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetResolveTarget(SharedPtr<Texture> &pTexture)
{
    mProperties->resolveTexture = pTexture;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetClearColor(float r, float g, float b, float a)
{
    mProperties->clearColor[0] = r;
    mProperties->clearColor[1] = g;
    mProperties->clearColor[2] = b;
    mProperties->clearColor[3] = a;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetLineWidth(float width)
{
    mProperties->lineWidth = width;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetMipIndex(int index)
{
    mProperties->mipIndex = index;
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetSamples(uint8_t samples)
{
    mProperties->samples = samples;
    return *this;
}

Pipeline::Ptr Pipeline::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
{
    return pContext->CreatePipeline(std::move(mProperties));
}

Pipeline::Pipeline(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}
Pipeline::~Pipeline()
{
    if (mProperties != nullptr)
        delete mProperties;
}
uint32_t Pipeline::GetWidth()
{
    if (mProperties->swapchainTarget)
    {
    }

    if (mProperties->colorTargets[0] != nullptr)
    {
        return mProperties->mipIndex > 0 ? mProperties->colorTargets[0]->GetWidth(mProperties->mipIndex) : mProperties->colorTargets[0]->GetWidth();
    }
    return 0;
}

uint32_t Pipeline::GetHeight()
{
    if (mProperties->swapchainTarget)
    {
    }

    if (mProperties->colorTargets[0] != nullptr)
    {
        return mProperties->mipIndex > 0 ? mProperties->colorTargets[0]->GetHeight(mProperties->mipIndex) : mProperties->colorTargets[0]->GetHeight();
    }
    return 0;
}