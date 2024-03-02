#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"
namespace NoobRenderer
{
    namespace component
    {
        struct Transform
        {
            glm::vec3 position;
            glm::vec3 rotate;
            glm::vec3 scale;
            glm::mat4 model;
            void CalculateModelMatrix()
            {
                model = glm::translate(glm::mat4(1.0f), position);
                model = glm::rotate(model, glm::radians(rotate.r), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(rotate.g), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(rotate.b), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, scale);
            }
            Transform() : position(glm::vec3(0.f)), rotate(glm::vec3(0.f)), scale(glm::vec3(1.f)) {}
            Transform(glm::vec3 _position, glm::vec3 _rotate, glm::vec3 _scale)
                : position(_position), rotate(_rotate), scale(_scale) {}
            SERIALIZATION_COMPONENT_ENABLE(Transform, position, rotate, scale)
            SERIALIZATION_FROM_TYPE_ANY(Transform)
        };

    } // namespace component

};
