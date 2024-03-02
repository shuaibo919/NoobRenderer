#include "Engine/Window.h"
#include <iostream>
using namespace NoobRenderer;

namespace NoobRenderer::io
{
    static std::vector<onResetFunc> onResetFuncVecs;
    static std::vector<onKeyFunc> onKeyFuncVecs;
    static std::vector<onMouseButtonFunc> onMouseButtonFuncVecs;
    static std::vector<onCursorPosFunc> onCursorPosFuncVecs;
    static std::vector<onWindowSizeFunc> onWindowSizeFuncVecs;
    static std::vector<onScrollFunc> onScrollFuncVecs;
    void GLFWImplCallbackOnReset(GLFWwindow *window)
    {
        for (auto &func : onResetFuncVecs)
            func();
    }
    void GLFWImplCallbackOnKey(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        for (auto &func : onKeyFuncVecs)
            func(key, scancode, action, mods);
    }
    void GLFWImplCallbackOnMouseButton(GLFWwindow *window, int button, int action, int mods)
    {
        for (auto &func : onMouseButtonFuncVecs)
            func(button, action, mods);
    }
    void GLFWImplCallbackOnCursorPos(GLFWwindow *window, double cursor_x, double cursor_y)
    {
        for (auto &func : onCursorPosFuncVecs)
            func(cursor_x, cursor_y);
    }
    void GLFWImplCallbackOnWindowSize(GLFWwindow *window, int width, int height)
    {
        for (auto &func : onWindowSizeFuncVecs)
            func(width, height);
    }
    void GLFWImplCallbackOnScroll(GLFWwindow *window, double cursor_x, double cursor_y)
    {
        for (auto &func : onScrollFuncVecs)
            func(cursor_x, cursor_y);
    }
}

EngineWindow::EngineWindow(EngineWindow::Size size, std::string_view title, int major_version, int minor_version)
    : m_size(size)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_size.width, m_size.height, title.data(), NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
EngineWindow::~EngineWindow()
{
}
GLFWwindow *EngineWindow::GetWindow()
{
    return m_window;
}
EngineWindow::Size EngineWindow::GetSize()
{
    return m_size;
}
void EngineWindow::Resize(unsigned int width, unsigned int height)
{
    m_size.width = width;
    m_size.height = height;
    RescaleViewport();
}
void EngineWindow::RescaleViewport()
{
    glViewport(0, 0, m_size.width, m_size.height);
}
void EngineWindow::RegisterIO()
{
    glfwSetFramebufferSizeCallback(m_window, io::GLFWImplCallbackOnWindowSize);
    glfwSetKeyCallback(m_window, io::GLFWImplCallbackOnKey);
    glfwSetCursorPosCallback(m_window, io::GLFWImplCallbackOnCursorPos);
    glfwSetMouseButtonCallback(m_window, io::GLFWImplCallbackOnMouseButton);
    glfwSetScrollCallback(m_window, io::GLFWImplCallbackOnScroll);
}
void EngineWindow::RegisterOnResetFunc(io::onResetFunc func)
{
    io::onResetFuncVecs.emplace_back(func);
}
void EngineWindow::RegisterOnKeyFunc(io::onKeyFunc func)
{
    io::onKeyFuncVecs.emplace_back(func);
}
void EngineWindow::RegisterOnMouseButtonFunc(io::onMouseButtonFunc func)
{
    io::onMouseButtonFuncVecs.emplace_back(func);
}
void EngineWindow::RegisterOnCursorPosFunc(io::onCursorPosFunc func)
{
    io::onCursorPosFuncVecs.emplace_back(func);
}
void EngineWindow::RegisterOnReSizeFunc(io::onWindowSizeFunc func)
{
    io::onWindowSizeFuncVecs.emplace_back(func);
}
void EngineWindow::RegisterOnScrollFunc(io::onScrollFunc func)
{
    io::onScrollFuncVecs.emplace_back(func);
}
void EngineWindow::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}
void EngineWindow::PollEvents()
{
    glfwPollEvents();
}
bool EngineWindow::ShouldClose()
{
    return glfwWindowShouldClose(m_window);
}
void EngineWindow::Terminate()
{
    glfwTerminate();
}