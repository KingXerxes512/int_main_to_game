#pragma once

#include <cmath>

namespace game
{

struct Vector3
{
    static Vector3 Normalize(const Vector3& v)
    {
        const auto length = std::hypot(v.x, v.y, v.z);
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

//constexpr bool operator==(const Vector3& v1, const Vector3& v2)
//{
//    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
//}

}
