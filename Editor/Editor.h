#pragma once
// Engine Source
#include "Engine/NoobEngine.h"
// Editor Source
#include "Application/ApplicationUI.h"

namespace NoobRenderer
{
    namespace Application
    {

        class Editor final : public NoobRenderer::Singleton<Editor>
        {
        private:
            ApplicationUI::Ptr ui{nullptr};

        public:
            Editor(token) {}
            ~Editor();
            bool Init(std::string_view name = "NoobRenderer");
            void Run();
        };

    }
}