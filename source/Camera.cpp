#include <cmath>

#include "Camera.h"
#include "Log.h"

namespace
{

game::Vector3 CreateDirection(float pitch, float yaw)
{
    return game::Vector3::Normalize(
        {std::cos(yaw) * std::cos(pitch), std::sin(pitch), std::sin(yaw) * std::cos(pitch)});
}

}

namespace game
{

Camera::Camera(
    const Vector3& position,
    const Vector3& lookAt,
    const Vector3& up,
    float fov,
    float width,
    float height,
    float nearPlane,
    float farPlane)
    : m_View(Mat4::LookAt(position, lookAt, up))
    , m_Projection(Mat4::Perspective(fov, width, height, nearPlane, farPlane))
    , m_Position(position)
    , m_Direction(lookAt)
    , m_Up(up)
    , m_Pitch{}
    , m_Yaw{}
{
}

Vector3 Camera::Direction() const
{
    return m_Direction;
}

Vector3 Camera::Right() const
{
    return Vector3::Normalize(Vector3::Cross(m_Direction, m_Up));
}

Vector3 Camera::Up() const
{
    return m_Up;
}

void Camera::AdjustYaw(float adjust)
{
    m_Yaw += adjust;
    m_Direction = CreateDirection(m_Pitch, m_Yaw);
    m_View = Mat4::LookAt(m_Position, m_Position + m_Direction, m_Up);
}

void Camera::AdjustPitch(float adjust)
{
    m_Pitch += adjust;
    m_Direction = CreateDirection(m_Pitch, m_Yaw);
    m_View = Mat4::LookAt(m_Position, m_Position + m_Direction, m_Up);
}

void Camera::Translate(const Vector3& translation)
{
    m_Position += translation;
    m_Direction = CreateDirection(m_Pitch, m_Yaw);
    m_View = Mat4::LookAt(m_Position, m_Position + m_Direction, m_Up);
}

std::span<const float> Camera::View() const
{
    return m_View.Data();
}

std::span<const float> Camera::Projection() const
{
    return m_Projection.Data();
}

}
