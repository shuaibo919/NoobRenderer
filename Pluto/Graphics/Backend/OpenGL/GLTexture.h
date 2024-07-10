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
            using Ptr = std::shared_ptr<GLTexture2D>;
            GLTexture2D(RenderContext *ctx, Properties *&&pProperties);
            GLTexture2D(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~GLTexture2D();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;

        protected:
            uint8_t *LoadTextureData(const std::string &path);
            uint32_t LoadTexture(uint8_t *data);
            uint32_t mHandle;
        };

        class GLTexture2DArray final : public Texture2DArray
        {
            friend class GLContext;

        public:
            using Ptr = std::shared_ptr<GLTexture2DArray>;
            GLTexture2DArray(RenderContext *ctx, Properties *&&pProperties);
            GLTexture2DArray(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~GLTexture2DArray();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;

        private:
            uint32_t mHandle;
        };

        class GLTextureCube final : public TextureCube
        {
            friend class GLContext;

        public:
            using Ptr = std::shared_ptr<GLTextureCube>;
            GLTextureCube(RenderContext *ctx, Properties *&&pProperties);
            GLTextureCube(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~GLTextureCube();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;

        private:
            uint32_t mHandle;
        };

        class GLTexture3D final : public Texture3D
        {
            friend class GLContext;

        public:
            using Ptr = std::shared_ptr<GLTexture3D>;
            GLTexture3D(RenderContext *ctx, Properties *&&pProperties);
            GLTexture3D(RenderContext *ctx, const std::string &path, Properties *&&pProperties);
            ~GLTexture3D();

        public:
            void *GetHandle() const override;
            void Bind(uint32_t slot = 0) const override;
            void Unbind(uint32_t slot = 0) const override;

        private:
            uint32_t mHandle;
        };
    }
}
