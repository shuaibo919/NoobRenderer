#include "Engine/NoobEngine.h"
using namespace NoobRenderer;
render::Renderer::Ptr Engine::GetRenderer(std::string name)
{
    if(m_renderers.find(name) == m_renderers.end())
        return nullptr;
    return m_renderers[name];
}