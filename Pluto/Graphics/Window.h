#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/SwapChain.h"
#include "Graphics/RHI/GraphicsContext.h"

namespace pluto
{
    class Window
    {
    public:
        static std::shared_ptr<Window> Create(std::shared_ptr<pluto::Graphics::GraphicsContext> &graphicsContext,
                                              unsigned int width, unsigned int height, const char *title);
        virtual ~Window();
        virtual int ShouldClose();
        virtual void Terminate();

        void PollEvents();
        void SwapBuffers();

        uint16_t GetWidth() const { return mWidth; };
        uint16_t GetHeight() const { return mHeight; };

        const std::shared_ptr<pluto::Graphics::SwapChain> &GetSwapChain() const { return mSwapChain; }
        const std::shared_ptr<pluto::Graphics::GraphicsContext> &GetGraphicsContext() const { return mGraphicsContext; }

        Window(std::shared_ptr<pluto::Graphics::GraphicsContext> &graphicsContext, unsigned int width, unsigned int height, const char *title);

    private:
        struct WindowImpl;
        uint16_t mWidth;
        uint16_t mHeight;
        std::string mTitle;
        WindowImpl *mImpl;

        std::shared_ptr<pluto::Graphics::SwapChain> mSwapChain;
        std::shared_ptr<pluto::Graphics::GraphicsContext> mGraphicsContext;
    };
}
