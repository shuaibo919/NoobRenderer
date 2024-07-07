#pragma once

#include "Graphics/RHI/Texture.h"

namespace pluto
{
    namespace Graphics
    {
        class GLTexture2D final : public Texture2D
        {
            friend class GLContext;

        public:
            GLTexture2D(Properties *&&pProperties);
            ~GLTexture2D();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class GLTexture2DArray final : public Texture2DArray
        {
            friend class GLContext;

        public:
            GLTexture2DArray(Properties *&&pProperties);
            ~GLTexture2DArray();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class GLTextureCube final : public TextureCube
        {
            friend class GLContext;

        public:
            GLTextureCube(Properties *&&pProperties);
            ~GLTextureCube();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };

        class GLTexture3D final : public Texture3D
        {
            friend class GLContext;

        public:
            GLTexture3D(Properties *&&pProperties);
            ~GLTexture3D();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;
        };
    }
}
