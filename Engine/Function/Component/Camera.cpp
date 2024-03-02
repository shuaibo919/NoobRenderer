#include "Engine/Function/Component/Camera.h"
#include <gtx/transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace NoobRenderer::component;
Camera::Camera(glm::vec3 positon, glm::vec3 lookat,
               float near, float far, float fov,
               float sensitivity, float speed, float max_pitch_rate, float max_heading_rate)
    : m_camera_position(positon), m_camera_look_at(lookat),
      m_near_clip(near), m_far_clip(far), m_field_of_view(fov),
      m_sensitivity(sensitivity), m_speed(speed), m_max_pitch_rate(max_pitch_rate),
      m_max_heading_rate(max_heading_rate)
{
    m_camera_up = glm::vec3(0, 1.f, 0);
    m_camera_position_delta = glm::vec3(0.f, 0, 0);
    m_camera_pitch = 0.0f;
    m_camera_heading = 0.0f;
}
void Camera::Tick(unsigned int src_width, unsigned int src_height)
{
    if (src_width == 0 || src_height == 0)
        return;
    // 相机朝向:单位方向向量(点)减去相机位置
    m_camera_direction = glm::normalize(m_camera_look_at - m_camera_position);
    m_aspect = static_cast<float>(src_width) / static_cast<float>(src_height);
    m_projection = glm::perspective(glm::radians(m_field_of_view), m_aspect, m_near_clip, m_far_clip);
    // detmine axis for pitch rotation
    glm::vec3 axis = glm::cross(m_camera_direction, m_camera_up);
    // compute quaternion for pitch based on the camera pitch angle
    glm::quat pitch_quat = glm::angleAxis(m_camera_pitch, axis);
    // determine heading quaternion from the camera up vector and the heading angle
    glm::quat heading_quat = glm::angleAxis(m_camera_heading, m_camera_up);
    // add the two quaternions
    glm::quat _tmp = glm::cross(pitch_quat, heading_quat);
    _tmp = glm::normalize(_tmp);
    // update the direction from the quaternion
    m_camera_direction = glm::rotate(_tmp, m_camera_direction);
    // add the camera delta
    m_camera_position += m_camera_position_delta;
    // set the look at to be infront of the camera
    m_camera_look_at = m_camera_position + m_camera_direction * 1.0f;
    // damping for smooth camera
    m_camera_heading *= .8f;
    m_camera_pitch *= .8f;
    m_camera_position_delta = m_camera_position_delta * .95f;

    m_view = glm::lookAt(m_camera_position, m_camera_look_at, m_camera_up);
}
void Camera::SetFOV(float fov)
{
    m_field_of_view = fov;
}
void Camera::SetPositionVector(glm::vec3 newPostion)
{
    m_camera_position = newPostion;
}
void Camera::SetPitch(float degrees)
{
    // Check bounds with the max pitch rate so that we aren't moving too fast
    if (degrees < -m_max_pitch_rate)
    {
        degrees = -m_max_pitch_rate;
    }
    else if (degrees > m_max_pitch_rate)
    {
        degrees = m_max_pitch_rate;
    }
    m_camera_pitch += degrees;

    // Check bounds for the camera pitch
    if (m_camera_pitch > 360.0f)
    {
        m_camera_pitch -= 360.0f;
    }
    else if (m_camera_pitch < -360.0f)
    {
        m_camera_pitch += 360.0f;
    }
}
void Camera::SetHeading(float degrees)
{
    // Check bounds with the max heading rate so that we aren't moving too fast
    if (degrees < -m_max_heading_rate)
    {
        degrees = -m_max_heading_rate;
    }
    else if (degrees > m_max_heading_rate)
    {
        degrees = m_max_heading_rate;
    }
    // This controls how the heading is changed if the camera is pointed straight up or down
    // The heading delta direction changes
    if (m_camera_pitch > 90 && m_camera_pitch < 270 || (m_camera_pitch < -90 && m_camera_pitch > -270))
    {
        m_camera_heading -= degrees;
    }
    else
    {
        m_camera_heading += degrees;
    }
    // Check bounds for the camera heading
    if (m_camera_heading > 360.0f)
    {
        m_camera_heading -= 360.0f;
    }
    else if (m_camera_heading < -360.0f)
    {
        m_camera_heading += 360.0f;
    }
}
void Camera::SetMode(PresetCameraMode cam_mode)
{
    m_camera_mode = cam_mode;
}
void Camera::SetLookAt(glm::vec3 pos)
{
    m_camera_look_at = pos;
}
void Camera::SetNear(float near_clip_distance)
{
    m_near_clip = near_clip_distance;
}
void Camera::SetFar(float far_clip_distance)
{
    m_far_clip = far_clip_distance;
}
void Camera::SetSensitivity(float val)
{
    m_sensitivity = val;
}
void Camera::SetSpeed(float val)
{
    m_speed = val;
}
glm::mat4 Camera::GetProjectionMatrix()
{
    return m_projection;
}
glm::mat4 Camera::GetViewMatrix()
{
    return m_view;
}
glm::vec3 Camera::GetRightVector()
{
    return glm::cross(m_camera_direction, m_camera_up);
}
glm::vec3 Camera::GetUpVector()
{
    return m_camera_up;
}
glm::vec3 Camera::GetFrontVector()
{
    return m_camera_direction;
}
glm::vec3 Camera::GetPositionVector()
{
    return m_camera_position;
}
PresetCameraMode Camera::GetMode()
{
    return m_camera_mode;
}
float Camera::GetNear()
{
    return m_near_clip;
}
float Camera::GetFar()
{
    return m_far_clip;
}
float Camera::GetFOV()
{
    return m_field_of_view;
}
float Camera::GetPitch()
{
    return m_camera_pitch;
}
float Camera::GetSensitivity()
{
    return m_sensitivity;
}
float Camera::GetSpeed()
{
    return m_speed;
}
float Camera::GetHeading()
{
    return m_camera_heading;
}

void Camera::Move(Camera::Action action)
{
    switch (action)
    {
    case Action::Up:
        m_camera_position_delta += m_camera_up * m_speed;
        break;
    case Action::Down:
        m_camera_position_delta -= m_camera_up * m_speed;
        break;
    case Action::Left:
        m_camera_position_delta -= glm::cross(m_camera_direction, m_camera_up) * m_speed;
        break;
    case Action::Right:
        m_camera_position_delta += glm::cross(m_camera_direction, m_camera_up) * m_speed;
        break;
    case Action::Forward:
        m_camera_position_delta += m_camera_direction * m_speed;
        break;
    case Action::Backward:
        m_camera_position_delta -= m_camera_direction * m_speed;
        break;
    }
}
void Camera::Scroll(float delta)
{
    m_camera_position_delta += m_camera_direction * m_speed * delta;
}
void Camera::Move2D(float x, float y)
{
    // compute the mouse delta from the previous mouse position
    glm::vec3 mouse_delta = m_mouse_position - glm::vec3(x, y, 0);
    // if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
    if (m_moving)
    {
        SetHeading(m_sensitivity * mouse_delta.x);
        SetPitch(m_sensitivity * mouse_delta.y);
    }
    m_mouse_position = glm::vec3(x, y, 0);
}
void Camera::SetMoving(bool val)
{
    m_moving = val;
}
bool Camera::GetMoving()
{
    return m_moving;
}