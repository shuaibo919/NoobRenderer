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
Framebuffer::Framebuffer(Properties *&&pProperties)
    : mProperties(pProperties)
{
}

struct FramebufferAsset
{
    std::shared_ptr<Framebuffer> framebuffer;
    float timeSinceLastAccessed;
};
static std::unordered_map<uint64_t, FramebufferAsset> m_FramebufferCache;
static const float m_CacheLifeTime = 1.0f;

// std::shared_ptr<Framebuffer> Framebuffer::Get(const FramebufferDesc &framebufferDesc)
// {
//     // uint64_t hash = 0;
//     // utilities::GetMixedHash(hash, framebufferDesc.attachmentCount, framebufferDesc.width, framebufferDesc.height, framebufferDesc.layer, framebufferDesc.screenFBO, framebufferDesc.samples);

//     // for (uint32_t i = 0; i < framebufferDesc.attachmentCount; i++)
//     // {
//     //     utilities::GetMixedHash(hash, framebufferDesc.attachmentTypes[i]);

//     //     if (framebufferDesc.attachments[i])
//     //     {
//     //         utilities::GetMixedHash(hash, framebufferDesc.attachments[i]->GetImageHande());
//     //         utilities::GetMixedHash(hash, framebufferDesc.attachments[i]->GetUUID());
//     //     }
//     // }

//     // auto found = m_FramebufferCache.find(hash);
//     // if (found != m_FramebufferCache.end() && found->second.framebuffer)
//     // {
//     //     found->second.timeSinceLastAccessed = (float)Engine::GetTimeStep().GetElapsedSeconds();
//     //     return found->second.framebuffer;
//     // }

//     // auto framebuffer = SharedPtr<Framebuffer>(Create(framebufferDesc));
//     // m_FramebufferCache[hash] = {framebuffer, (float)Engine::GetTimeStep().GetElapsedSeconds()};
//     // return framebuffer;
//     return nullptr;
// }

void Framebuffer::ClearCache()
{
    m_FramebufferCache.clear();
}

void Framebuffer::DeleteUnusedCache()
{
    // static const size_t keyDeleteSize = 256;
    // static std::size_t keysToDelete[keyDeleteSize];
    // std::size_t keysToDeleteCount = 0;

    // for (auto &&[key, value] : m_FramebufferCache)
    // {
    //     if (!value.framebuffer)
    //     {
    //         keysToDelete[keysToDeleteCount] = key;
    //         keysToDeleteCount++;
    //     }
    //     else if (value.framebuffer.GetCounter() && value.framebuffer.GetCounter()->GetReferenceCount() == 1 && (Engine::GetTimeStep().GetElapsedSeconds() - value.timeSinceLastAccessed) > m_CacheLifeTime)
    //     {
    //         keysToDelete[keysToDeleteCount] = key;
    //         keysToDeleteCount++;
    //     }

    //     if (keysToDeleteCount >= keyDeleteSize)
    //         break;
    // }

    // for (std::size_t i = 0; i < keysToDeleteCount; i++)
    // {
    //     m_FramebufferCache[keysToDelete[i]].framebuffer = nullptr;
    //     m_FramebufferCache.erase(keysToDelete[i]);
    // }
}

Framebuffer::~Framebuffer()
{
}
