#include "Engine/NoobEngine.h"
#include "Engine/Window.h"
#include <iostream>
#include <fstream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main()
{
    using namespace NoobRenderer;
    EngineWindow::Size wsize{1280, 720};
    auto test_window = std::make_shared<EngineWindow>(wsize, "test", 4, 6);
    {
        Engine::Instance().Init();
        while (!test_window->ShouldClose())
        {
            test_window->PollEvents();
            {
                // ImGui::ShowDemoWindow();
            }
            test_window->SwapBuffers();
        }
        test_window->Terminate();
    }
    return 0;
}