#pragma once
#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{
    class CubeMapTexture : public TextureBase
    {

    private:
        std::vector<std::string> faces;

    public:
        using Ptr = std::shared_ptr<CubeMapTexture>;
        CubeMapTexture(unsigned int width, unsigned int height, Texture::Format format, Texture::Format interformat,
                       Texture::DataType datatype);
        CubeMapTexture() = delete;

    private:
        void SettingTexture() override;

    public:
        // inline std::vector<std::string> &GetFaces() { return faces; }
    };
}