#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
namespace NoobRenderer
{

    class TextureBase;
    namespace Texture
    {
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

    // Wrapper Class of glTexParameteri including basic common settings;
    class TextureParameter
    {
    private:
        std::vector<std::pair<gtype::TexParaType, gtype::TexPara>> m_paras;

    public:
        TextureParameter();
        void Add(gtype::TexParaType type, gtype::TexPara para);
        void Apply(GLenum target);
    };

    class TextureBase
    {
    protected:
        GLuint m_id;
        int m_width, m_height, m_tmp_slot;
        Texture::Type m_type;
        gtype::TexType m_basetype;
        gtype::Format m_format;
        gtype::Format m_internalformat;
        gtype::DataType m_datatype;

    protected:
        TextureParameter m_params;
        void TexImage2D(GLenum target, GLint level, GLint border, const void *pixels);
        void TexImage3D(GLenum target, GLint level, GLint border, int layers, const void *pixels);
        void TexImage2DMultisample(GLenum target, GLsizei samples, GLboolean fixedsamplelocations);
        // void SetParameter(gtype::TexParaType parameter, GLint * &value);
        inline void InitTextureUnits() { glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Texture::helper::textureUnits); }
        virtual void SettingTexture() = 0;

    public:
        using Ptr = std::shared_ptr<TextureBase>;
        TextureBase() = delete;
        TextureBase(int width, int height, Texture::Type type, gtype::TexType basetype,
                    gtype::Format format, gtype::Format internalformat, gtype::DataType datatype);
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

    public:
        inline GLuint GetID() const { return m_id; }
        inline Texture::Type &GetType() { return m_type; }
        inline gtype::TexType &GetBaseType() { return m_basetype; }
        inline gtype::Format GetFormat() const { return m_format; }
        inline gtype::Format GetInternalFormat() const { return m_internalformat; }
        inline gtype::DataType GetDataType() const { return m_datatype; }
        inline int GetTempSlot() { return m_tmp_slot; }
        inline int GetWidth() const { return m_width; }
        inline int GetHeight() const { return m_height; }
        inline void SetBaseParameter(TextureParameter val) { m_params = val; }
        void SetParameter(gtype::TexParaType parameter, GLfloat value);
        void SetParameter(gtype::TexParaType parameter, GLfloat *value);
    };
}
