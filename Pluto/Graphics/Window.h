#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/SwapChain.h"
#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    class Window
    {
    public:
        static SharedPtr<Window> Create(const SharedPtr<Graphics::GraphicsContext> &graphicsContext,
                                        unsigned int width, unsigned int height, const char *title);
        virtual ~Window();
        virtual int ShouldClose();
        virtual void Terminate();

        void PollEvents();
        void SwapBuffers();

        uint16_t GetWidth() const { return mWidth; };
        uint16_t GetHeight() const { return mHeight; };

        const SharedPtr<Graphics::SwapChain> &GetSwapChain() const { return mSwapChain; }
        const SharedPtr<Graphics::GraphicsContext> &GetGraphicsContext() const { return mGraphicsContext; }

        Window(const SharedPtr<Graphics::GraphicsContext> &graphicsContext, unsigned int width, unsigned int height, const char *title);

    private:
        struct WindowImpl;
        uint16_t mWidth;
        uint16_t mHeight;
        std::string mTitle;
        WindowImpl *mImpl;

        SharedPtr<Graphics::SwapChain> mSwapChain;
        SharedPtr<Graphics::GraphicsContext> mGraphicsContext;
    };
}
