#include "gtest/gtest.h"

#include "Vector3.h"

TEST(Vector3, negate)
{
    auto a = -game::Vector3{.x = 1.0f, .y = 2.0f, .z = 3.0f};
    auto b = game::Vector3{.x = -1.0f, .y = -2.0f, .z = -3.0f};
    ASSERT_EQ(a, b);
}