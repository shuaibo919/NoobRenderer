#pragma once

#include "Graphics/RHI/Texture.h"

namespace pluto
{
    namespace Graphics
    {
        class VKTexture2D final : public Texture2D
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTexture2D>;
            VKTexture2D(RenderContext *ctx, Properties *&&pProperties);
            VKTexture2D(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTexture2D();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class VKTexture2DArray final : public Texture2DArray
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTexture2DArray>;
            VKTexture2DArray(RenderContext *ctx, Properties *&&pProperties);
            VKTexture2DArray(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTexture2DArray();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class VKTextureCube final : public TextureCube
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTextureCube>;
            VKTextureCube(RenderContext *ctx, Properties *&&pProperties);
            VKTextureCube(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTextureCube();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class VKTexture3D final : public Texture3D
        {
            friend class VKContext;

        public:
            using Ptr = SharedPtr<VKTexture3D>;
            VKTexture3D(RenderContext *ctx, Properties *&&pProperties);
            VKTexture3D(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~VKTexture3D();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };
    }
}
