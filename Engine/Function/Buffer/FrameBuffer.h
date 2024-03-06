#pragma once
#include "Engine/Function/Buffer/BufferBase.h"
namespace NoobRenderer
{
    /**
     * @brief 帧缓冲,构造完后默认已绑定
     */
    class FrameBuffer : public BufferBase
    {
    public:
        using Ptr = std::shared_ptr<FrameBuffer>;

        FrameBuffer();
        ~FrameBuffer();

    protected:
        void InitBuffer() override;

    public:
        void ToTexture(int attachment, int texturePrimitive, int textureID);
        void BlitFrom(FrameBuffer::Ptr &frameBuffer, unsigned int width, unsigned int height);
        void SetRenderBuffer(unsigned int attachment, unsigned int renderBufferID);
        void SetTexture2D(unsigned int attachment, unsigned int textureId, GLenum textarget = GL_TEXTURE_2D, int level = 0);
        void SetTexture3D(unsigned int attachment, unsigned int textureId, int level = 0, int zoffset = 0);
        void SetTexture(unsigned int attachment, unsigned int textureId, int level = 0);
        void Bind() override;
        void Unbind() override;
        bool IsComplete();
    };
}