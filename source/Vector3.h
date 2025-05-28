#pragma once

#include <cmath>
#include <format>

namespace game
{

struct Vector3
{
    static Vector3 Normalize(const Vector3& v)
    {
        const auto length = std::hypot(v.x, v.y, v.z);

        if (length == 0.0)
        {
            return {.x = 0.0f, .y = 0.0f, .z = 0.0f};
        }

        return {.x = v.x / length, .y = v.y / length, .z = v.z / length};
    }

    static constexpr Vector3 Cross(const Vector3& v1, const Vector3& v2)
    {
        auto i = (v1.y * v2.z) - (v1.z * v2.y);
        auto j = (v1.x * v2.z) - (v1.z * v2.x);
        auto k = (v1.x * v2.y) - (v1.y * v2.x);

        return {.x = i, .y = -j, .z = k};
    }

    constexpr bool operator==(const Vector3&) const = default;

    float x;
    float y;
    float z;
};

constexpr Vector3& operator*=(Vector3& v, float f)
{
    v.x *= f;
    v.y *= f;
    v.z *= f;

    return v;
}

constexpr Vector3 operator*(const Vector3& v, float f)
{
    auto tmp = v;
    return tmp *= f;
}

constexpr Vector3& operator+=(Vector3& v1, const Vector3& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;

    return v1;
}

constexpr Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;

    return v1;
}

constexpr Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    auto tmp = v1;
    return tmp -= v2;
}

constexpr Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    auto tmp = v1;
    return tmp += v2;
}

constexpr Vector3 operator-(const Vector3& v)
{
    return {-v.x, -v.y, -v.z};
}

}

template <>
struct std::formatter<game::Vector3>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::begin(ctx);
    }

    auto format(const game::Vector3& obj, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "x={}, y={}, z={}", obj.x, obj.y, obj.z);
    }
};
