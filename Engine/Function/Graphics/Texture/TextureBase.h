#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
namespace NoobRenderer
{

    class TextureBase;
    namespace Texture
    {
        enum class BaseType : GLenum
        {
            Tex2D = GL_TEXTURE_2D,
            Tex3D = GL_TEXTURE_3D,
            Tex2DArray = GL_TEXTURE_2D_ARRAY,
            Tex2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
            TexCube = GL_TEXTURE_CUBE_MAP
        };

        enum class Format : GLint
        {
            NONE = 0,
            RGBA = GL_RGBA,
            RGB = GL_RGB,
            RG = GL_RG,
            RED = GL_RED,
            DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
            RGBA32F = GL_RGBA32F,
            RGB32F = GL_RGB32F,
            RG32F = GL_RG32F,
            R32F = GL_R32F,
            RGBA8 = GL_RGBA8,
            RGBA16 = GL_RGBA16,
            RGBA16F = GL_RGBA16F,
            RGB16F = GL_RGB16F,
            RG16F = GL_RG16F,
            R16F = GL_R16F,
        };

        enum class DataType : GLenum
        {
            UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
            FLOAT = GL_FLOAT
        };

        enum class Type : int
        {
            None = -1,
            TextureAmbient = 0,
            TextureDiffuse = 1,
            TextureSpecular = 2,
            TextureNormal = 3,
            TextureHeight = 4,
            TextureEmission = 5,
            TextureDepth = 6,
            TextureColorBuffer = 7,
            TextureCubeMap = 8,
            TextureAlbedo = 9,
            TextureMetallic = 10,
            TextureRoughness = 11,
            TextureAmbientOcclusion = 12,
            TextureHDR = 13,
        };
        class helper
        {
        public:
            friend class NoobRenderer::TextureBase;
            friend void ResetSlot();
            friend int UseSlot();
            friend const int GetSlot();
            friend void SpecifySlot(int);

        private:
            helper() = default;
            inline static constexpr std::array<std::string_view, 15> type_2_string{
                "TextureAmbient",
                "TextureDiffuse",
                "TextureSpecular",
                "TextureNormal",
                "TextureHeight",
                "TextureEmission",
                "TextureDepth",
                "TextureColorBuffer",
                "TextureCubeMap",
                "TextureAlbedo",
                "TextureMetallic",
                "TextureRoughness",
                "TextureAmbientOcclusion",
                "TextureHDR",
                "None"};
            inline static int textureUnits{0};
            inline static int slotCount{0};
        };
        inline int UseSlot() { return helper::slotCount++; }
        inline const int GetSlot() { return helper::slotCount; }
        inline void ResetSlot() { helper::slotCount = 0; }
        inline void SpecifySlot(int slot) { helper::slotCount = slot; }
        inline void Active(int slot) { glActiveTexture(GL_TEXTURE0 + slot); }
    }

    struct TextureParamStroage
    {
        std::unordered_map<GLenum, GLfloat> paramf;
        std::unordered_map<GLenum, GLfloat *> paramfv;
        std::unordered_map<GLenum, GLint> parami;
        std::unordered_map<GLenum, GLint *> paramiv;
    };

    class TextureBase
    {
    protected:
        GLuint m_id;
        int m_width, m_height, m_tmp_slot;
        Texture::Type m_type;
        Texture::BaseType m_basetype;
        Texture::Format m_format;
        Texture::Format m_internalformat;
        Texture::DataType m_datatype;
        TextureParamStroage m_params;

    protected:
        void TexImage2D(GLenum target, GLint level, GLint border, const void *pixels);
        void TexImage3D(GLenum target, GLint level, GLint border, int depth, const void *pixels);
        void TexImage2DMultisample(GLenum target, GLsizei samples, GLboolean fixedsamplelocations);
        inline void InitTextureUnits() { glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Texture::helper::textureUnits); }
        virtual void SettingTexture() = 0;

    public:
        using Ptr = std::shared_ptr<TextureBase>;
        TextureBase() = delete;
        TextureBase(int width, int height, Texture::Type type, Texture::BaseType basetype,
                    Texture::Format format, Texture::Format internalformat, Texture::DataType datatype);
        TextureBase(const TextureBase &texture);
        TextureBase(TextureBase &&texture) noexcept;
        virtual ~TextureBase();
        void Activate();
        void Bind();
        void Unbind();
        void Rescale(unsigned int width, unsigned int height);
        void Deactivate();
        void GenerateMipmap();
        std::string_view GetTypeString();
        void TexStorage2D(GLenum target, GLsizei levels);
        void TexStorage3D(GLenum target, GLsizei levels, GLsizei depth);
        void SetParameter(GLenum pname, GLfloat value);
        void SetParameter(GLenum pname, GLfloat *value);
        void SetParameter(GLenum pname, GLint value);
        void SetParameter(GLenum pname, GLint *value);
        void SetParameterAndSave(GLenum pname, GLfloat value);
        void SetParameterAndSave(GLenum pname, GLfloat *value);
        void SetParameterAndSave(GLenum pname, GLint value);
        void SetParameterAndSave(GLenum pname, GLint *value);
        void ClearTexImage(GLint level, const void *pixels);
        void BindImage(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

    public:
        inline GLuint GetID() const { return m_id; }
        inline Texture::Type &GetType() { return m_type; }
        inline Texture::BaseType &GetBaseType() { return m_basetype; }
        inline Texture::Format GetFormat() const { return m_format; }
        inline Texture::Format GetInternalFormat() const { return m_internalformat; }
        inline Texture::DataType GetDataType() const { return m_datatype; }
        inline int GetTempSlot() { return m_tmp_slot; }
        inline int GetWidth() const { return m_width; }
        inline int GetHeight() const { return m_height; }
    };
}
