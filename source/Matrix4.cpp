#include "Matrix4.h"

namespace game
{

game::Mat4 Mat4::LookAt(const Vector3& eye, const Vector3& look_at, const Vector3& up)
{
    const auto f = Vector3::Normalize(look_at - eye);
    const auto up_normalized = Vector3::Normalize(up);

    const auto s = Vector3::Normalize(Vector3::Cross(f, up_normalized));
    const auto u = Vector3::Normalize(Vector3::Cross(s, f));

    Mat4 m;
    m.m_Elements = {{s.x, u.x, -f.x, 0.0f, s.y, u.y, -f.y, 0.0f, s.z, u.z, -f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

    return m * Mat4(-eye);
}

game::Mat4 Mat4::Perspective(float fov, float width, float height, float near_plane, float far_plane)
{
    Mat4 m;

    const auto aspect_ratio = width / height;
    const auto tmp = std::tanf(fov / 2.0f);
    const auto t = tmp * near_plane;
    const auto b = -t;
    const auto r = t * aspect_ratio;
    const auto l = b * aspect_ratio;

    m.m_Elements = {
        {(2.0f * near_plane) / (r - l),
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         (2.0f * near_plane) / (t - b),
         0.0f,
         0.0f,
         (r + l) / (r - l),
         (t + b) / (t - b),
         -(far_plane + near_plane) / (far_plane - near_plane),
         -1.0f,
         0.0f,
         0.0f,
         -(2.0f * far_plane * near_plane) / (far_plane - near_plane),
         0.0f}};

    return m;
}

}
