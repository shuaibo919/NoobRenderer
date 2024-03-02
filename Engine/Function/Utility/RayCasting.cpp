#include "Engine/Function/Utility/RayCasting.h"
namespace EngineUtility
{
    glm::vec3 GetNormalisedDeviceCoordinates(float mouse_x, float mouse_y, int screen_width, int screen_height)
    {
        float ndc_x = (2.0f * mouse_x) / (float)screen_width - 1.0f;
        float ndc_y = 1.0f - ((2.0f * mouse_y) / (float)screen_height);
        float ndc_z = 1.0;
        return glm::vec3(ndc_x, ndc_y, ndc_z);
    }

    glm::vec4 ToEyeCoords(glm::vec4 &ray_clip, glm::mat4 &projection_matrix)
    {
        glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
        // Now, we only needed to un-project the x,y part, so let's manually set the z,w part to mean "forwards, and not a point"
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
        return ray_eye;
    }

    glm::vec3 ToWorldCoords(glm::vec4 &ray_eye, glm::mat4 &view_matrix)
    {
        glm::vec3 ray_world = glm::inverse(view_matrix) * ray_eye;
        ray_world = glm::normalize(ray_world);
        return ray_world;
    }
}