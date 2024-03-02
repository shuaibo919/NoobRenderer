#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/Serialization.h"

namespace NoobRenderer
{
    namespace component
    {
        enum class PresetCameraMode
        {
            FreeView,
            Invalid
        };

        class Camera
        {
        public:
            enum class Action
            {
                Forward,
                Backward,
                Left,
                Right,
                Up,
                Down,
            };

            using Ptr = std::shared_ptr<Camera>;
            Camera(glm::vec3 positon = glm::vec3(0, 1.5f, 3.f), glm::vec3 lookat = glm::vec3(0, 0, -1.f),
                   float near = 0.1, float far = 150, float fov = 90.0,
                   float sensitivity = 0.001f, float speed = 0.1f, float max_pitch_rate = 5.f, float max_heading_rate = 5.f);
            /**
             * @brief This function updates the camera
             * Depending on the current camera mode, the projection and viewport matricies are computed
             * Then the position and location of the camera is updated
             */
            void Tick(unsigned int src_width, unsigned int src_height);

            /**
             * @brief Change the heading and pitch of the camera based on the 2d movement of the mouse
             * @param x mouse offset-x
             * @param y mouse offset-y
             */
            // void Move2D(int x, int y);

            // Getting Functions
            glm::mat4 GetProjectionMatrix();
            glm::mat4 GetViewMatrix();
            glm::vec3 GetRightVector();
            glm::vec3 GetUpVector();
            glm::vec3 GetFrontVector();
            glm::vec3 GetPositionVector();
            PresetCameraMode GetMode();
            float GetNear();
            float GetFar();
            float GetFOV();
            float GetHeading();
            float GetPitch();
            float GetSensitivity();
            float GetSpeed();

            // Setting Functions
            void SetPositionVector(glm::vec3 newPostion); // Set the position of the camera
            void SetPitch(float degrees);                 // Change the pitch (up, down) for the free camera
            void SetHeading(float degrees);               // Change heading (left, right) for the free camera
            void SetMode(PresetCameraMode cam_mode);      // Changes the camera mode, only three valid modes, Ortho, Free, and Spherical
            void SetLookAt(glm::vec3 pos);                // Set's the look at point for the camera
            void SetFOV(float fov);                       // Changes the Field of View (FOV) for the camera
            void SetNear(float near_clip_distance);
            void SetFar(float far_clip_distance);
            void SetSensitivity(float val);
            void SetSpeed(float val);

            // Convenient Functions
            void Move(Camera::Action action);
            void Scroll(float delta);
            void Move2D(float x, float y);
            void SetMoving(bool val);
            bool GetMoving();

            PresetCameraMode m_camera_mode = PresetCameraMode::FreeView;

            bool m_moving{false};

            float m_aspect;
            float m_field_of_view;
            float m_near_clip;
            float m_far_clip;

            float m_camera_heading;
            float m_camera_pitch;
            float m_sensitivity;
            float m_speed;

            float m_max_pitch_rate;
            float m_max_heading_rate;

            glm::vec3 m_camera_position;
            glm::vec3 m_camera_position_delta;
            glm::vec3 m_camera_look_at;
            glm::vec3 m_camera_direction;
            glm::vec3 m_mouse_position;

            glm::vec3 m_camera_up;
            glm::mat4 m_view;
            glm::mat4 m_projection;

            SERIALIZATION_COMPONENT_ENABLE(Camera, m_aspect, m_field_of_view, m_near_clip, m_far_clip,
                                           m_camera_heading, m_camera_pitch, m_sensitivity, m_speed, m_max_pitch_rate, m_max_heading_rate,
                                           m_camera_position, m_camera_position_delta, m_camera_look_at, m_camera_direction,
                                           m_camera_up, m_view, m_projection)
            SERIALIZATION_FROM_TYPE_ANY(Camera)
        };
    } // namespace component

};
