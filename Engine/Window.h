#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
#include "GLFW/glfw3.h"
namespace NoobRenderer
{
    namespace io
    {
        using onResetFunc = std::function<void()>;
        using onKeyFunc = std::function<void(int, int, int, int)>;
        using onMouseButtonFunc = std::function<void(int, int, int)>;
        using onCursorPosFunc = std::function<void(double, double)>;
        using onScrollFunc = std::function<void(double, double)>;
        using onWindowSizeFunc = std::function<void(int, int)>;
    }

    class EngineWindow : public std::enable_shared_from_this<EngineWindow>
    {

    public:
        struct Size
        {
            unsigned int width, height;
        };

    protected:
        GLFWwindow *m_window{nullptr};
        EngineWindow::Size m_size;

    public:
        EngineWindow() = delete;
        EngineWindow(EngineWindow::Size size, std::string_view title, int major_version, int minor_version);
        ~EngineWindow();
        GLFWwindow *GetWindow();
        EngineWindow::Size GetSize();
        void Resize(unsigned int width, unsigned int height);
        void RescaleViewport();
        void SwapBuffers();
        void PollEvents();
        bool ShouldClose();
        void Terminate();
        void RegisterIO();
        void RegisterOnResetFunc(io::onResetFunc func);
        void RegisterOnKeyFunc(io::onKeyFunc func);
        void RegisterOnMouseButtonFunc(io::onMouseButtonFunc func);
        void RegisterOnCursorPosFunc(io::onCursorPosFunc func);
        void RegisterOnReSizeFunc(io::onWindowSizeFunc func);
        void RegisterOnScrollFunc(io::onScrollFunc func);
    };
}
