#pragma once

#include <array>
#include <format>
#include <span>

#include "Vector3.h"

namespace game
{

class Matrix4
{
  public:
    constexpr Matrix4()
        : m_Elements({1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f})
    {
    }

    constexpr Matrix4(const std::array<float, 16u>& elements)
        : m_Elements(elements)
    {
    }

    constexpr Matrix4(const Vector3& translation)
        : m_Elements(
              {1.0f,
               0.0f,
               0.0f,
               0.0f,
               0.0f,
               1.0f,
               0.0f,
               0.0f,
               0.0f,
               0.0f,
               1.0f,
               0.0f,
               translation.x,
               translation.y,
               translation.z,
               1.0f})
    {
    }

    constexpr Matrix4(const Vector3& translation, const Vector3& scale)
        : m_Elements(
              {scale.x,
               0.0f,
               0.0f,
               0.0f,
               0.0f,
               scale.y,
               0.0f,
               0.0f,
               0.0f,
               0.0f,
               scale.z,
               0.0f,
               translation.x,
               translation.y,
               translation.z,
               1.0f})
    {
    }

    static Matrix4 LookAt(const Vector3& eye, const Vector3& look_at, const Vector3& up);
    static Matrix4 Perspective(float fov, float width, float height, float near_plane, float far_plane);

    constexpr std::span<const float> Data() const
    {
        return m_Elements;
    }

    friend constexpr Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

    constexpr bool operator==(const Matrix4&) const = default;

  private:
    std::array<float, 16u> m_Elements;
};

constexpr Matrix4& operator*=(Matrix4& m1, const Matrix4& m2)
{
    Matrix4 result = {};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.m_Elements[i + j * 4] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                result.m_Elements[i + j * 4] += m1.m_Elements[i + k * 4] * m2.m_Elements[k + j * 4];
            }
        }
    }

    m1 = result;
    return m1;
}

constexpr Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
    Matrix4 tmp = m1;
    return tmp *= m2;
}

}

template <>
struct std::formatter<game::Matrix4>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::begin(ctx);
    }

    auto format(const game::Matrix4& obj, std::format_context& ctx) const
    {
        const auto* data = obj.Data().data();
        return std::format_to(
            ctx.out(),
            "{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}",
            data[0],
            data[4],
            data[8],
            data[12],
            data[1],
            data[5],
            data[9],
            data[13],
            data[2],
            data[6],
            data[10],
            data[14],
            data[3],
            data[7],
            data[11],
            data[15]);
    }
};
