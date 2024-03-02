#pragma once
#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{

    class MultiSampleTexture : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<MultiSampleTexture>;
        MultiSampleTexture(unsigned int width, unsigned int height, gtype::Format internalformat, int samples,
                           const Texture::Type &type = Texture::Type::TextureColorBuffer);
        MultiSampleTexture() = delete;

    private:
        void SettingTexture() override;
    };
}
