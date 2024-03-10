#pragma once
#include "Engine/NoobEngine.h"
namespace NoobRenderer
{
    namespace Application
    {
        class DrawPickedMeshOutlinePass
        {
        private:
            WriteToTexture::Ptr m_rt{nullptr};
            WriteToTexture::Ptr m_edge{nullptr};
            ShaderProgram::Ptr m_shader{nullptr};
            ShaderProgram::Ptr m_sobel_shader{nullptr};
            glm::vec3 m_edge_color{0.5, 0.5f, 0.1f};

        public:
            using Ptr = std::shared_ptr<DrawPickedMeshOutlinePass>;
            void Init(unsigned int width, unsigned int height);
            void Rescale(unsigned int width, unsigned int height);
            void Render(Scene::Node::Ptr &node);
        };
    }
}
