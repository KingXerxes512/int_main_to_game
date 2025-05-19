#include "Camera.h"

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
{
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
