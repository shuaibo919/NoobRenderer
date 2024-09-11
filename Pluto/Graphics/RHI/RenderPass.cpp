#include "Graphics/RHI/RenderPass.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;
struct RenderPassAsset
{
    // SharedPtr<RenderPass> renderpass;
    // float timeSinceLastAccessed;
};
static std::unordered_map<uint64_t, RenderPassAsset> m_RenderPassCache;
static const float m_CacheLifeTime = 0.1f;

RenderPass::Builder &RenderPass::Builder::SetAttachment(SharedPtr<Texture> &&texture, AttachmentType type)
{
    mProperties->attachments.push_back(std::forward<SharedPtr<Texture>>(texture));
    mProperties->attachmentTypes.push_back(type);
    return *this;
}

RenderPass::Builder &RenderPass::Builder::SetClear(bool clear)
{
    mProperties->clear = clear;
    return *this;
}

RenderPass::Builder &RenderPass::Builder::SetSwapchainTarget(bool swapchainTarget)
{
    mProperties->swapchainTarget = swapchainTarget;
    return *this;
}

RenderPass::Builder &RenderPass::Builder::SetCubeMapIndex(int index)
{
    mProperties->cubeMapIndex = index;
    return *this;
}

RenderPass::Builder &RenderPass::Builder::SetSamples(uint32_t samples)
{
    mProperties->samples = samples;
    return *this;
}

RenderPass::Builder &RenderPass::Builder::SetMipIndex(int index)
{
    mProperties->mipIndex = index;
    return *this;
}
RenderPass::Builder &RenderPass::Builder::SetResolveTexture(SharedPtr<Texture> &&texture)
{
    mProperties->resolveTexture = std::forward<SharedPtr<Texture>>(texture);
    return *this;
}

RenderPass::Ptr RenderPass::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
{
    return pContext->CreateRenderPass(std::move(mProperties));
}

RenderPass::RenderPass(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

RenderPass::~RenderPass()
{
    if (mProperties != nullptr)
        delete mProperties;
}
// SharedPtr<RenderPass> RenderPass::Get(const RenderPassDesc &renderPassDesc)
// {
//     // uint64_t hash = 0;
//     // GetMixedHash(hash, renderPassDesc.attachmentCount, renderPassDesc.clear, renderPassDesc.cubeMapIndex, renderPassDesc.mipIndex, renderPassDesc.samples);

//     // for (uint32_t i = 0; i < renderPassDesc.attachmentCount; i++)
//     // {
//     //     GetMixedHash(hash, (uint32_t)renderPassDesc.attachmentTypes[i]);

//     //     if (renderPassDesc.attachments[i])
//     //         GetMixedHash(hash, renderPassDesc.attachments[i]->GetUUID());
//     // }

//     // if (renderPassDesc.resolveTexture)
//     //     GetMixedHash(hash, renderPassDesc.resolveTexture->GetUUID());

//     // auto found = m_RenderPassCache.find(hash);
//     // if (found != m_RenderPassCache.end() && found->second.renderpass)
//     // {
//     //     found->second.timeSinceLastAccessed = (float)Engine::GetTimeStep().GetElapsedSeconds();
//     //     return found->second.renderpass;
//     // }

//     // auto renderPass = SharedPtr<RenderPass>(Create(renderPassDesc));
//     // m_RenderPassCache[hash] = {renderPass, (float)Engine::GetTimeStep().GetElapsedSeconds()};
//     // return renderPass;
//     return nullptr;
// }

void RenderPass::ClearCache()
{
    m_RenderPassCache.clear();
}

void RenderPass::DeleteUnusedCache()
{
    // static std::size_t keysToDelete[256];
    // std::size_t keysToDeleteCount = 0;

    // for (auto &&[key, value] : m_RenderPassCache)
    // {
    //     if (value.renderpass && value.renderpass.GetCounter() && value.renderpass.GetCounter()->GetReferenceCount() == 1 && (Engine::GetTimeStep().GetElapsedSeconds() - value.timeSinceLastAccessed) > m_CacheLifeTime)
    //     {
    //         keysToDelete[keysToDeleteCount] = key;
    //         keysToDeleteCount++;
    //     }

    //     if (keysToDeleteCount >= 256)
    //         break;
    // }

    // for (std::size_t i = 0; i < keysToDeleteCount; i++)
    // {
    //     m_RenderPassCache[keysToDelete[i]].renderpass = nullptr;
    //     m_RenderPassCache.erase(keysToDelete[i]);
    // }
}
// void RenderPass::BeginRenderPass(CommandBuffer *commandBuffer, float *clearColour, Framebuffer *frame, SubPassContents contents, uint32_t width, uint32_t height) const
// {
// }