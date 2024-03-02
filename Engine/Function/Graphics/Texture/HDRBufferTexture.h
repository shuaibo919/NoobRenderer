#pragma once
#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{

    class HDRBufferTexture : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<HDRBufferTexture>;
        HDRBufferTexture(const std::string &path);

    private:
        void SettingTextureFromFile(const std::string &path);
        void SettingTexture() override;
        // void GenerateTexture() override;
    };
}