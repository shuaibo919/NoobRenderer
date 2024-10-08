#include "Graphics/Window.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
/* Common */
#include "Core/Log.hpp"
/* Third */
#include "GLFW/glfw3.h"
#if defined(VULKAN_BACKEND)
#define GLFW_INCLUDE_VULKAN
#endif
using namespace pluto;

struct Window::WindowImpl
{
    WindowImpl() : mWindow(nullptr) {}
    GLFWwindow *mWindow;
    static Window::WindowImpl *CreateOpenGLWindow(unsigned int width, unsigned int height, const char *title, int hint = 4, int value = 1)
    {
        auto wImpl = new Window::WindowImpl();
#if defined(OPENGL_BACKEND)
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
#endif
        return wImpl;
    }
    static Window::WindowImpl *CreateVulkanWindow(unsigned int width, unsigned int height, const char *title)
    {
        auto wImpl = new Window::WindowImpl();
#if defined(VULKAN_BACKEND)
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        wImpl->mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
#endif
        return wImpl;
    }
    ~WindowImpl()
    {
        if (mWindow == nullptr)
        {
            glfwDestroyWindow(mWindow);
        }
    }
};

SharedPtr<Window> Window::Create(const SharedPtr<pluto::Graphics::GraphicsContext> &graphicsContext,
                                 unsigned int width, unsigned int height, const char *title)
{
    return std::make_shared<Window>(graphicsContext, width, height, title);
}

Window::Window(const SharedPtr<pluto::Graphics::GraphicsContext> &graphicsContext, unsigned int width, unsigned int height, const char *title)
    : mWidth(width), mHeight(height), mTitle(title), mImpl(nullptr)
{
    auto api = graphicsContext->GetRenderAPI();
    switch (api)
    {
    case Graphics::RenderAPI::OPENGL:
        mImpl = Window::WindowImpl::CreateOpenGLWindow(mWidth, mHeight, mTitle.c_str());
        break;
    case Graphics::RenderAPI::VULKAN:
        mImpl = Window::WindowImpl::CreateVulkanWindow(mWidth, mHeight, mTitle.c_str());
        break;

    default:
        break;
    }
}

Window::~Window()
{
}

Graphics::SwapChain::Properties Window::GetSwapChainProperties()
{
    auto props = Graphics::SwapChain::Properties();
    props.height = mHeight;
    props.width = mWidth;
    props.window = this;
    return props;
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
    delete mImpl;
}

void Window::PollEvents()
{
    glfwPollEvents();
}
void Window::SwapBuffers()
{
    glfwSwapBuffers(mImpl->mWindow);
}
void *Window::GetWindowPointer()
{
    return mImpl->mWindow;
}