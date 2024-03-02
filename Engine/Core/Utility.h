#pragma once
#include <cmath>
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"

namespace EngineUtility
{
    template <class T, class U>
    concept TisBasedOf = std::is_base_of<U, T>::value;
    static const float Math_PI = 3.14159265358979323846264338327950288f;
    static const float Math_ONE_OVER_PI = 1.0f / Math_PI;
    static const float Math_TWO_PI = 2.0f * Math_PI;
    static const float Math_HALF_PI = 0.5f * Math_PI;
    static const float Math_DEG2RAD = Math_PI / 180.0f;
    static const float Math_RAD2DEG = (180.f / Math_PI);

    float DegreeToRadian(float degree);
    float Arctan(float x);
    float Tan(float x);

    glm::vec3 DecomposePositonFromMatrix(const glm::mat4 &m);
    glm::vec3 DecomposeScaleFromMatrix(const glm::mat4 &m);
    glm::vec3 DecomposeRotationFromMatrix(const glm::mat4 &m);

    float Max(float a, float b);
    glm::vec3 Max(glm::vec3 a, glm::vec3 b);
    glm::vec4 Max(glm::vec4 a, glm::vec4 b);
    float Min(float a, float b);
    glm::vec3 Min(glm::vec3 a, glm::vec3 b);
    glm::vec4 Min(glm::vec4 a, glm::vec4 b);

    std::string GetFileName(const std::string &path);
    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4 &proj, const glm::mat4 &view);

    float lerp(float a, float b, float f);
}