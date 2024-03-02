#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"

namespace NoobRenderer
{
    namespace component
    {
        struct Light
        {
            bool shadow{false};
            glm::vec3 position;
            glm::vec3 color;
            Light() : color(glm::vec3(1.0f)), position(glm::vec3(0.0f)) {}
            SERIALIZATION_COMPONENT_ENABLE(Light, position, color, shadow)
            SERIALIZATION_FROM_TYPE_ANY(Light)
        };

        struct PointLight : public Light
        {
            float constant{1.0f};
            float linear{0.009f};
            float quadratic{0.0032f};
            float near{1.0f};
            float far{100.0f};
            int shadow_channel{0};
            PointLight() : Light() {}
            SERIALIZATION_COMPONENT_ENABLE(PointLight, position, color, constant, linear, quadratic, near, far, shadow, shadow_channel)
            SERIALIZATION_FROM_TYPE_ANY(PointLight)
        };

        /**
         * @brief Note:not use the position property
         */
        struct DirectionalLight : public Light
        {
            glm::vec3 direction{20.0f, 50.0f, 20.0f};
            float intensity{0.5f};
            /* cascade shadow */
            std::vector<float> cascade_splits{30.f, 15.f, 5.f, 2.f, 1.f};
            std::vector<float> biasModifier{0.1f, 0.13f, 0.3f, 0.64f, 1.2f};
            std::vector<float> cascade_levels;
            float biasFactor{0.1f};
            float minBias{0.05f};
            float shadowRadius{6.0f};
            /* volumetric */
            float vScatter{0.75f};
            float vIntensity{0.5f};
            int vStep{10};

            DirectionalLight() : Light() {}
            SERIALIZATION_COMPONENT_ENABLE(DirectionalLight, direction, color, shadow, cascade_splits, intensity, biasFactor, biasModifier,
                                           minBias, shadowRadius, vScatter, vStep, vIntensity)
            SERIALIZATION_FROM_TYPE_ANY(DirectionalLight)
        };

        struct AreaLight : public Light
        {
            float constant{1.0f};
            float linear{0.009f};
            float quadratic{0.0032f};
            std::vector<glm::vec3> positions;
            AreaLight() : Light() {}
            SERIALIZATION_COMPONENT_ENABLE(AreaLight, color, positions, color, constant, linear, quadratic)
            SERIALIZATION_FROM_TYPE_ANY(AreaLight)
        };
        struct EnvironmentLight : public Light
        {
        };

    } // namespace component

};
