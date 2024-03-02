#include <iostream>
#include "Editor.h"
int main(int argc, char **argv)
{
    using namespace NoobRenderer;
    if (!Application::Editor::Instance().Init("NoobRenderer"))
    {
        std::cout << "Main:: Failed to init!" << std::endl;
        return -1;
    }
    Application::Editor::Instance().Run();
    return 0;
}