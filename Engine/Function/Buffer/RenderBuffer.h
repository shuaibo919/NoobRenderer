#pragma once
#include "Engine/Function/Buffer/BufferBase.h"
namespace NoobRenderer
{
    /**
     * @brief 渲染缓冲,构造完后默认绑定
     */
    class RenderBuffer : public BufferBase
    {
    public:
        using Ptr = std::shared_ptr<RenderBuffer>;

        RenderBuffer(unsigned int width, unsigned int height, int internalFormat = GL_DEPTH_COMPONENT);
        RenderBuffer() = default;
        virtual ~RenderBuffer();

    protected:
        unsigned int m_width, m_height;
        int m_internalFormat;

    protected:
        void InitBuffer() override;

    public:
        void Bind() override;
        void Unbind() override;
        virtual void Rescale(unsigned int width, unsigned int height);

    public:
        inline unsigned int GetWidth() const { return m_width; }
        inline unsigned int GetHeight() const { return m_height; }
        inline int GetInternalFormat() const { return m_internalFormat; }
    };

    class MultiSampleRenderBuffer : public RenderBuffer
    {
    public:
        MultiSampleRenderBuffer(unsigned int width, unsigned int height, int internalFormat, int samples = 4);
        MultiSampleRenderBuffer() = delete;
        void Rescale(unsigned int width, unsigned int height) override;

    protected:
        void InitBuffer() override;

    private:
        int m_samples;
    };
}