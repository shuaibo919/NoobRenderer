#include "Graphics/RHI/Pipeline.h"
#include "Graphics/RHI/GraphicsContext.h"
#include "Core/Utilities.h"

// #ifdef OPENGL_BACKEND
// #include "Graphics/Backend/OpenGL/GLSwapChain.h"
// #endif
// #ifdef VULKAN_BACKEND
// #include "Graphics/Backend/Vulkan/VKTexture.h"
// #endif
using namespace pluto::Graphics;

struct PipelineAsset
{
    std::shared_ptr<Pipeline> pipeline;
    float timeSinceLastAccessed;
};
static std::unordered_map<uint64_t, PipelineAsset> m_PipelineCache;
static const float m_CacheLifeTime = 0.1f;

Pipeline::Builder &Pipeline::Builder::SetShader(std::shared_ptr<Shader> &pShader)
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

Pipeline::Builder &Pipeline::Builder::SetColourTarget(std::shared_ptr<Texture> &pTexture)
{
    mProperties->colourTargets.fill(pTexture);
    return *this;
}

Pipeline::Builder &Pipeline::Builder::SetResolveTarget(std::shared_ptr<Texture> &pTexture)
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

Pipeline::Ptr Pipeline::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreatePipeline(std::move(mProperties));
}

Pipeline::Pipeline(Properties *&&pProperties)
    : mProperties(pProperties)
{
}
Pipeline::~Pipeline()
{
    if (mProperties != nullptr)
        delete mProperties;
}
// std::shared_ptr<Pipeline> Pipeline::Get(const PipelineDesc &pipelineDesc)
// {
// uint64_t hash = 0;
// utilities::GetMixedHash(hash, pipelineDesc.shader.get(), pipelineDesc.cullMode, pipelineDesc.depthBiasEnabled, (uint32_t)pipelineDesc.drawType, (uint32_t)pipelineDesc.polygonMode, pipelineDesc.transparencyEnabled, pipelineDesc.DepthTest, pipelineDesc.DepthWrite);

// for (auto texture : pipelineDesc.colourTargets)
// {
//     if (texture)
//     {
//         utilities::GetMixedHash(hash, texture->GetUUID());
//     }
// }

// if (pipelineDesc.resolveTexture)
// {
//     utilities::GetMixedHash(hash, pipelineDesc.resolveTexture->GetUUID());
// }

// if (pipelineDesc.depthTarget)
// {
//     utilities::GetMixedHash(hash, pipelineDesc.depthTarget->GetUUID());
// }

// if (pipelineDesc.depthArrayTarget)
// {
//     utilities::GetMixedHash(hash, pipelineDesc.depthArrayTarget->GetUUID());
// }

// utilities::GetMixedHash(hash, pipelineDesc.clearTargets);
// utilities::GetMixedHash(hash, pipelineDesc.swapchainTarget);
// utilities::GetMixedHash(hash, pipelineDesc.lineWidth);
// if (pipelineDesc.depthBiasEnabled)
// {
//     utilities::GetMixedHash(hash, pipelineDesc.depthBiasConstantFactor);
//     utilities::GetMixedHash(hash, pipelineDesc.depthBiasSlopeFactor);
// }

// if (pipelineDesc.cubeMapTarget)
// {
//     utilities::GetMixedHash(hash, pipelineDesc.cubeMapIndex);
//     utilities::GetMixedHash(hash, pipelineDesc.cubeMapTarget->GetUUID());
// }

// utilities::GetMixedHash(hash, pipelineDesc.mipIndex);
// utilities::GetMixedHash(hash, pipelineDesc.samples);

// if (pipelineDesc.swapchainTarget)
// {
//     // Add one swapchain image to hash
//     auto texture = Renderer::GetMainSwapChain()->GetCurrentImage();
//     if (texture)
//     {
//         utilities::GetMixedHash(hash, texture->GetUUID());
//     }
// }

// auto found = m_PipelineCache.find(hash);
// if (found != m_PipelineCache.end() && found->second.pipeline)
// {
//     found->second.timeSinceLastAccessed = (float)Engine::GetTimeStep().GetElapsedSeconds();
//     return found->second.pipeline;
// }

// std::shared_ptr<Pipeline> pipeline = std::shared_ptr<Pipeline>(Create(pipelineDesc));
// m_PipelineCache[hash] = {pipeline, (float)Engine::GetTimeStep().GetElapsedSeconds()};
// return pipeline;
// return nullptr;
// }

void Pipeline::ClearCache()
{
    m_PipelineCache.clear();
}

void Pipeline::DeleteUnusedCache()
{
    // static std::size_t keysToDelete[256];
    // std::size_t keysToDeleteCount = 0;

    // for (auto &&[key, value] : m_PipelineCache)
    // {
    //     if (value.pipeline && value.pipeline.GetCounter()->GetReferenceCount() == 1 && (Engine::GetTimeStep().GetElapsedSeconds() - value.timeSinceLastAccessed) > m_CacheLifeTime)
    //     {
    //         keysToDelete[keysToDeleteCount] = key;
    //         keysToDeleteCount++;
    //     }

    //     if (keysToDeleteCount >= 256)
    //         break;
    // }

    // for (std::size_t i = 0; i < keysToDeleteCount; i++)
    // {
    //     m_PipelineCache[keysToDelete[i]].pipeline = nullptr;
    //     m_PipelineCache.erase(keysToDelete[i]);
    // }
}

// uint32_t Pipeline::GetWidth()
// {
//     return 0;
//     if (m_Description.swapchainTarget)
//     {
// #ifdef LUMOS_RENDER_API_OPENGL
//         if (GraphicsContext::GetRenderAPI() == RenderAPI::OPENGL)
//             return ((GLSwapChain *)Renderer::GetMainSwapChain())->GetWidth();
// #endif
//         return Renderer::GetMainSwapChain()->GetCurrentImage()->GetWidth();
//     }

//     if (m_Description.colourTargets[0])
//     {
//         return m_Description.mipIndex > 0 ? m_Description.colourTargets[0]->GetWidth(m_Description.mipIndex) : m_Description.colourTargets[0]->GetWidth();
//     }

//     if (m_Description.depthTarget)
//         return m_Description.depthTarget->GetWidth();

//     if (m_Description.depthArrayTarget)
//         return m_Description.depthArrayTarget->GetWidth();

//     if (m_Description.cubeMapTarget)
//     {
//         return m_Description.mipIndex > 0 ? m_Description.cubeMapTarget->GetWidth(m_Description.mipIndex) : m_Description.cubeMapTarget->GetWidth();
//     }

//     LUMOS_LOG_WARN("Invalid pipeline width");

// return 0;
// }

// uint32_t Pipeline::GetHeight()
// {
//     if (m_Description.swapchainTarget)
//     {
// #ifdef LUMOS_RENDER_API_OPENGL
//         if (GraphicsContext::GetRenderAPI() == RenderAPI::OPENGL)
//             return ((GLSwapChain *)Renderer::GetMainSwapChain())->GetHeight();
// #endif
//         return Renderer::GetMainSwapChain()->GetCurrentImage()->GetHeight();
//     }

//     if (m_Description.colourTargets[0])
//     {
//         // TODO
//         return m_Description.mipIndex > 0 ? m_Description.colourTargets[0]->GetHeight(m_Description.mipIndex) : m_Description.colourTargets[0]->GetHeight();
//     }

//     if (m_Description.depthTarget)
//         return m_Description.depthTarget->GetHeight();

//     if (m_Description.depthArrayTarget)
//         return m_Description.depthArrayTarget->GetHeight();

//     if (m_Description.cubeMapTarget)
//     {
//         return m_Description.mipIndex > 0 ? m_Description.cubeMapTarget->GetHeight(m_Description.mipIndex) : m_Description.cubeMapTarget->GetHeight();
//     }

//     LUMOS_LOG_WARN("Invalid pipeline height");

// return 0;
// }