#include "Editor.h"
#include <iostream>
using namespace NoobRenderer::Application;

bool Editor::Init(std::string_view name)
{
    EngineWindow::Size wsize = {1284, 768};
    ui = std::make_shared<ApplicationUI>(wsize, name, 4, 6);
    if (!ui->Init("#version 130"))
    {
        std::cout << "EditorApplication:: Failed to initialize UI!" << std::endl;
        return false;
    }
    Engine::Instance().Init();
    inspection::Init();
    Engine::Instance().AddRenderer<render::DeferredRenderer>("Deferred Renderer", wsize.width / 2, wsize.height / 2);
    //Engine::Instance().AddRenderer<render::ForwardPBRRenderer>("Forward Renderer", wsize.width / 2, wsize.height / 2);
    Engine::Instance().SetCurrentRenderer("Deferred Renderer");
    ui->RegisterIO();
    return true;
}

void Editor::Run()
{
    if (ui == nullptr)
    {
        std::cout << "EditorApplication::Ui is null!" << std::endl;
        return;
    }

    while (!ui->ShouldClose())
    {
        ui->PollEvents();
        {
            if (ui->GetReadyToRendering())
            {
                Engine::Instance().GetCurrentRenderer()->Resize(ui->GetRenderingSize().width, ui->GetRenderingSize().height);
                Engine::Instance().UpdateCurrentScene();
                Engine::Instance().UpdateCurrentRenderer();
            }
            ui->RescaleViewport();
            ui->UpdateUI();
        }
        ui->SwapBuffers();
    }
    ui->Terminate();
}

Editor::~Editor()
{
    if (ui != nullptr)
        ui.reset();
}