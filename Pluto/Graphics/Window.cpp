#include "Graphics/Window.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
/* Common */
#include "Core/Log.hpp"
/* Third */
#include "GLFW/glfw3.h"
using namespace pluto;

struct Window::WindowImpl
{
    WindowImpl() : mWindow(nullptr) {}
    GLFWwindow *mWindow;
    static Window::WindowImpl *CreateOpenGLWindow(unsigned int width, unsigned int height, const char *title, int hint = 4, int value = 1)
    {
        auto wImpl = new Window::WindowImpl();
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, hint);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, value);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        wImpl->mWindow = glfwCreateWindow(width, height, title, NULL, NULL);
        if (wImpl->mWindow == nullptr)
        {
            log<Error>("Failed to create GLFW window");
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(wImpl->mWindow);
        if (!Graphics::GLContext::LoadGladProc((void *)glfwGetProcAddress))
        {
            log<Error>("Failed to initialize GLAD");
            return nullptr;
        }
        glfwSetInputMode(wImpl->mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return wImpl;
    }
    static Window::WindowImpl *CreateVulkanWindow(unsigned int width, unsigned int height, const char *title)
    {
        log<Error>("Not Implemented");
        return nullptr;
    }
    ~WindowImpl()
    {
        if (mWindow == nullptr)
        {
            glfwDestroyWindow(mWindow);
        }
    }
};

std::shared_ptr<Window> Window::Create(std::shared_ptr<pluto::Graphics::GraphicsContext> &graphicsContext,
                                       unsigned int width, unsigned int height, const char *title)
{
    return std::make_shared<Window>(graphicsContext, width, height, title);
}

Window::Window(std::shared_ptr<pluto::Graphics::GraphicsContext> &graphicsContext, unsigned int width, unsigned int height, const char *title)
    : mGraphicsContext(graphicsContext), mWidth(width), mHeight(height), mTitle(title), mImpl(nullptr)
{
    auto api = mGraphicsContext->GetRenderAPI();
    switch (api)
    {
    case Graphics::RenderAPI::OPENGL:
        mImpl = Window::WindowImpl::CreateOpenGLWindow(mWidth, mHeight, mTitle.c_str());
        break;
    case Graphics::RenderAPI::VULKAN:
        log<Error>("Not Implemented");
        break;

    default:
        break;
    }
    mSwapChain = Graphics::SwapChain::Builder()
                     .SetBase(mWidth, mHeight)
                     .SetWindow(this)
                     .Create(mGraphicsContext);
}

Window::~Window()
{
    Terminate();
    delete mImpl;
}

int Window::ShouldClose()
{
    if (mImpl->mWindow == nullptr)
        return 0;
    return glfwWindowShouldClose(mImpl->mWindow);
}

void Window::Terminate()
{
    if (mImpl->mWindow == nullptr)
        return;
    glfwTerminate();
}

void Window::PollEvents()
{
    glfwPollEvents();
}
void Window::SwapBuffers()
{
    glfwSwapBuffers(mImpl->mWindow);
}
