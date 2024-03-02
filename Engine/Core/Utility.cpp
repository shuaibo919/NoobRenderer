#include "Engine/Core/Utility.h"
#include <algorithm>
#include "Third/glm/gtx/orthonormalize.hpp"
namespace EngineUtility
{

    float DegreeToRadian(float degree)
    {
        return degree * Math_DEG2RAD;
    }

    float Arctan(float x)
    {
        return std::atanh(x);
    }

    float Tan(float x)
    {
        return std::tanh(x);
    }

    glm::vec3 DecomposePositonFromMatrix(const glm::mat4 &m)
    {
        return glm::vec3(m[3]);
    }
    glm::vec3 DecomposeScaleFromMatrix(const glm::mat4 &m)
    {
        return glm::vec3(glm::length(glm::vec3(m[0])), glm::length(glm::vec3(m[1])), glm::length(glm::vec3(m[2])));
    }
    glm::vec3 DecomposeRotationFromMatrix(const glm::mat4 &m)
    {
        // auto scale = DecomposeScaleFromMatrix(m);
        // const glm::mat3 rotMtx(
        //     glm::vec3(m[0]) / scale[0],
        //     glm::vec3(m[1]) / scale[1],
        //     glm::vec3(m[2]) / scale[2]);
        auto rotation = glm::quat_cast(m);
        return glm::eulerAngles(rotation);
        // rotation = glm::conjugate(rotation);
        // auto mat = glm::orthonormalize(glm::mat3(m));
        // return Math_RAD2DEG * glm::vec3(
        //                           atan2f(mat[1][2], mat[2][2]),
        //                           atan2f(-mat[0][2], sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2])),
        //                           atan2f(mat[0][1], mat[0][0]));
    }

    float Max(float a, float b)
    {
        return std::max(a, b);
    }

    glm::vec3 Max(glm::vec3 a, glm::vec3 b)
    {
        return glm::max(a, b);
    }
    glm::vec4 Max(glm::vec4 a, glm::vec4 b)
    {
        return glm::max(a, b);
    }

    float Min(float a, float b)
    {
        return std::min(a, b);
    }

    glm::vec3 Min(glm::vec3 a, glm::vec3 b)
    {
        return glm::min(a, b);
    }
    glm::vec4 Min(glm::vec4 a, glm::vec4 b)
    {
        return glm::min(a, b);
    }

    std::string GetFileName(const std::string &path)
    {
        auto start = path.find_last_of("/\\") + 1;
        return path.substr(start, path.size() - start);
    }
    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4 &proj, const glm::mat4 &view)
    {
        std::vector<glm::vec4> frustumCorners;
        auto inv = glm::inverse(proj * view);
        for (int x = 0; x < 2; x++)
        {
            for (int y = 0; y < 2; y++)
            {
                for (int z = 0; z < 2; z++)
                {
                    glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }
        return frustumCorners;
    }
    float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }
}