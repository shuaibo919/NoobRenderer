#pragma once
#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{

    class DepthTexture : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<DepthTexture>;
        DepthTexture(int width, int height);
        DepthTexture() = delete;

    private:
        void SettingTexture() override;
    };

    class DepthTextureArray : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<DepthTextureArray>;
        DepthTextureArray(int width, int height, int layers);
        DepthTextureArray() = delete;
        inline int GetLayers() const { return m_layers; }

    private:
        int m_layers;
        void SettingTexture() override;
    };
}