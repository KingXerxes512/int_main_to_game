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

    std::span<const float> View() const;
    std::span<const float> Projection() const;

  private:
    Mat4 m_View;
    Mat4 m_Projection;
};

}
