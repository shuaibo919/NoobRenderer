#pragma once
#include "Engine/Core/Utility.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Graphics/Texture/TextureBase.h"
namespace NoobRenderer
{

    class WriteToTexture
    {
    private:
        unsigned int m_width{0}, m_height{0};
        unsigned int rbo_attachment{0};
        std::vector<unsigned int> tex_attachments;
        std::vector<TextureBase::Ptr> m_texs;
        FrameBuffer::Ptr m_fbo{nullptr};
        RenderBuffer::Ptr m_rbo{nullptr};

    public:
        using Ptr = std::shared_ptr<WriteToTexture>;
        WriteToTexture() = delete;
        WriteToTexture(unsigned int width, unsigned int height);
        ~WriteToTexture();

    public:
        void ExplicitColorAttach();
        void DiscardRenderBuffer();
        void BlitFrom(const WriteToTexture::Ptr &write2tex);
        TextureBase::Ptr &GetTexture(int idx = 0);
        void ActivateTex(int idx = 0);
        void DeactivateTex(int tex_idx = 0);
        int GetTexSlot(int tex_idx = 0);
        GLint GetTexId(int tex_idx = 0);
        void Rescale(unsigned int width, unsigned int height);
        size_t GetTextureSize();

    public:
        template <EngineUtility::TisBasedOf<RenderBuffer> T, typename... Args>
        inline void SetRenderBuffer(unsigned int attachment, Args &&...args)
        {
            rbo_attachment = attachment;
            m_rbo = std::make_shared<T>(std::forward<Args>(args)...);
            m_fbo->SetRenderBuffer(rbo_attachment, m_rbo->GetID());
            m_rbo->Unbind();
        }

        template <EngineUtility::TisBasedOf<TextureBase> T, typename... Args>
        inline void SetTexture2D(unsigned int attachment, Args &&...args)
        {
            tex_attachments.push_back(attachment);
            m_texs.push_back(std::make_shared<T>(std::forward<Args>(args)...));
            m_fbo->SetTexture2D(attachment, m_texs.back()->GetID(), static_cast<GLenum>(m_texs.back()->GetBaseType()));
            m_texs.back()->Unbind();
        }

        template <EngineUtility::TisBasedOf<TextureBase> T, typename... Args>
        inline void SetTexture3D(unsigned int attachment, Args &&...args)
        {
            tex_attachments.push_back(attachment);
            m_texs.push_back(std::make_shared<T>(std::forward<Args>(args)...));
            m_fbo->SetTexture3D(attachment, m_texs.back()->GetID(), static_cast<GLenum>(m_texs.back()->GetBaseType()));
            m_texs.back()->Unbind();
        }

        template <EngineUtility::TisBasedOf<TextureBase> T, typename... Args>
        inline void SetTexture(unsigned int attachment, Args &&...args)
        {
            // Rescale For this Type Not Implmented!
            tex_attachments.push_back(attachment);
            m_texs.push_back(std::make_shared<T>(std::forward<Args>(args)...));
            m_fbo->SetTexture(attachment, m_texs.back()->GetID());
            m_texs.back()->Unbind();
        }

        inline void Unbind() { m_fbo->Unbind(); }
        inline void Bind() { m_fbo->Bind(); }
        inline unsigned int GetWidth() const { return m_width; }
        inline unsigned int GetHeight() const { return m_height; }
        auto begin() { return m_texs.begin(); }
        auto end() { return m_texs.end(); }
    };
}