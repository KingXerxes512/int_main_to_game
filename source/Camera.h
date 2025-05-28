#pragma once

#include <span>

#include "Matrix4.h"
#include "Vector3.h"

namespace game
{

class Camera
{
  public:
    Camera(
        const Vector3& position,
        const Vector3& lookAt,
        const Vector3& up,
        float fov,
        float width,
        float height,
        float nearPlane,
        float farPlane);

    Vector3 Direction() const;
    Vector3 Right() const;
    Vector3 Up() const;

    void AdjustYaw(float adjust);
    void AdjustPitch(float adjust);
    void Translate(const Vector3& translation);
    std::span<const float> View() const;
    std::span<const float> Projection() const;

  private:
    Mat4 m_View;
    Mat4 m_Projection;
    Vector3 m_Position;
    Vector3 m_Direction;
    Vector3 m_Up;
    float m_Pitch;
    float m_Yaw;
};

}
