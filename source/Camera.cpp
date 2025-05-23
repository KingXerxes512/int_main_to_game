#include "Camera.h"
#include "Log.h"

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
{
}

void Camera::Translate(const Vector3& translation)
{
    m_Position += translation;
    m_Direction += translation;
    m_View = Mat4::LookAt(m_Position, m_Direction, m_Up);
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
