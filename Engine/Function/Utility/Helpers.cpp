#include "Engine/Function/Utility/Helpers.h"
#include "Engine/Function/Component/All.h"
namespace EngineUtility
{
    void DrawQuad()
    {
        using namespace NoobRenderer;
        static bool FisrtIn = true;
        static Mesh::Ptr quad{nullptr};
        if (FisrtIn)
        {
            FisrtIn = false;
            std::vector<NoobRenderer::Vertex> vertices(6, NoobRenderer::Vertex());
            vertices[0].Position = glm::vec3(-1.f, 1.f, 0.f);
            vertices[0].TexCoords = glm::vec2(0.f, 1.f);
            vertices[1].Position = glm::vec3(-1.0f, -1.0f, 0.f);
            vertices[1].TexCoords = glm::vec2(0.0f, 0.0f);
            vertices[2].Position = glm::vec3(1.0f, -1.0f, 0.f);
            vertices[2].TexCoords = glm::vec2(1.0f, 0.0f);
            vertices[3].Position = glm::vec3(-1.0f, 1.0f, 0.f);
            vertices[3].TexCoords = glm::vec2(0.0f, 1.0f);
            vertices[4].Position = glm::vec3(1.0f, -1.0f, 0.f);
            vertices[4].TexCoords = glm::vec2(1.0f, 0.0f);
            vertices[5].Position = glm::vec3(1.0f, 1.0f, 0.f);
            vertices[5].TexCoords = glm::vec2(1.0f, 1.0f);
            quad = std::make_shared<NoobRenderer::Mesh>(vertices);
            quad->SetPrimitive(DrawPrimitive::Triangle);
        }
        {
            glDisable(GL_DEPTH_TEST);
            quad->Draw();
            glEnable(GL_DEPTH_TEST);
        }
    }
}