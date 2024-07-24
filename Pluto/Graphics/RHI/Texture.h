#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class Texture : public Asset, public RHIBase
        {
            friend class GraphicsContext;

        public:
            using Ptr = SharedPtr<Texture>;
            enum class Type
            {
                Invalid,
                Texture2D,
                Texture2DArray,
                Texture3D,
                TextureCube
            };
            struct Properties
            {
                uint16_t width;
                uint16_t height;
                RHIFormat format;
                TextureFilter minFilter;
                TextureFilter magFilter;
                TextureWrap wrap;
                uint16_t samples{1};
                uint16_t zAxisSize{1};
                TextureFlags flags{TextureFlags::TextureCreateMips};
                Texture::Type type{Texture::Type::Invalid};
                bool srgb{false};
                bool generateMipMaps{true};
                bool anisotropicFiltering{true};
            };
            struct Builder final : BuildBase<Texture::Properties, Texture>
            {
                Builder() {}
                Texture::Builder &SetBase(uint16_t width, uint16_t height, uint16_t samples, RHIFormat format, uint16_t zAxisSize = 1);
                Texture::Builder &SetFilter(TextureFilter minFilter, TextureFilter magFilter);
                Texture::Builder &SetWrap(TextureWrap wrap);
                Texture::Builder &SetAdancedOptions(TextureFlags flag = TextureFlags::TextureCreateMips, bool srgb = true, bool mipmap = true, bool anisotropic = true);
                Texture::Ptr Create(Texture::Type type, const SharedPtr<GraphicsContext> &pContext);
                Texture::Ptr Create(Texture::Properties &desc, const SharedPtr<GraphicsContext> &pContext);
                Texture::Ptr Create(Texture::Type type, const std::string &path, const SharedPtr<GraphicsContext> &pContext);
                Texture::Ptr Create(const std::string &path, Texture::Properties &desc, const SharedPtr<GraphicsContext> &pContext);
            };

        public:
            virtual ~Texture();
            virtual void *GetHandle() const = 0;
            virtual void Bind(uint32_t slot = 0) const = 0;
            virtual void Unbind(uint32_t slot = 0) const = 0;

        public:
            // SET_ASSET_TYPE(AssetType::Texture);
            Identity GetIdentity() const { return mIdentity; }
            const Properties &GetProperties() const { return *mProperties; }
            uint32_t GetWidth(uint32_t mip = 0) const { return mProperties->width >> mip; }
            uint32_t GetHeight(uint32_t mip = 0) const { return mProperties->height >> mip; }

        protected:
            Properties *mProperties;
            Texture(RenderContext *ctx, Properties *&&pProperties);
            Identity mIdentity{0};
        };

        class Texture2D : public Texture
        {
        public:
            virtual ~Texture2D();

        protected:
            Texture2D(RenderContext *ctx, Properties *&&pProperties);
        };

        class Texture2DArray : public Texture
        {
        public:
            virtual ~Texture2DArray();

        protected:
            Texture2DArray(RenderContext *ctx, Properties *&&pProperties);
        };

        class TextureCube : public Texture
        {
        public:
            virtual ~TextureCube();

        protected:
            TextureCube(RenderContext *ctx, Properties *&&pProperties);
        };

        class Texture3D : public Texture
        {
        public:
            virtual ~Texture3D();

        protected:
            Texture3D(RenderContext *ctx, Properties *&&pProperties);
        };

    }
}
