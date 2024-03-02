#pragma once
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "Engine/Core/GraphicsAPI.h"
namespace EngineUtility
{
    /**
     * @brief 世界坐标转换 (主要用于Ray Casting)
     * @param ray_eye 光线入射点
     * @param view_matrix 视图矩阵
     * @return 世界坐标
     */
    glm::vec3 ToWorldCoords(glm::vec4 &ray_eye, glm::mat4 &view_matrix);

    /**
     * @brief normally, to get into clip space from eye space we multiply the vector by a projection matrix,
     *        we can go backwards by multiplying by the inverse of this matrix.(主要用于Ray Casting)
     * @param ray_clip
     * @param projection_matrix 投影矩阵
     * @return eye space 坐标
     */
    glm::vec4 ToEyeCoords(glm::vec4 &ray_clip, glm::mat4 &projection_matrix);

    /**
     * @brief Get Normalised Device Coordinates(主要用于Ray Casting)
     * @param mouse_x 鼠标x点
     * @param mouse_y 鼠标y点
     * @param screen_width 屏幕宽度
     * @param screen_height 屏幕高度
     * @return 标准化设备坐标
     */
    glm::vec3 GetNormalisedDeviceCoordinates(float mouse_x, float mouse_y, int screen_width, int screen_height);

    /**
     * @brief 光线投射记录
     */
    class RayCastingRecord
    {
    private:
        bool m_hitted;
        float m_t;
        glm::vec3 m_hit_position{ 0.f }; // 平面与光线交点
        glm::vec3 m_plane_normal{ 0.f }; // 平面法线
    public:
        RayCastingRecord() : m_hitted(false), m_t(std::numeric_limits<float>::max()) {}
        RayCastingRecord(float t, glm::vec3 hit_position, glm::vec3 plane_normal)
            : m_hitted(true), m_t(t), m_hit_position(hit_position), m_plane_normal(plane_normal) {}
        glm::vec3 GetHitPosition() { return m_hit_position; }
        glm::vec3 GetPlaneNormal() { return m_plane_normal; }
        float GetRayFactor() { return m_t; }
        bool GetHitted() { return m_hitted; }
    };

}