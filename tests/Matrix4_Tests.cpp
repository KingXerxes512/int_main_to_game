#include "gtest/gtest.h"

#include "Matrix4.h"

#include <numbers>
#include <print>

TEST(Matrix4, Multiply)
{
    const auto m1 = game::Mat4{{
        1.0f, //
        2.0f,
        3.0f,
        4.0f,
        1.0f, //
        2.0f,
        3.0f,
        4.0f,
        1.0f, //
        2.0f,
        3.0f,
        4.0f,
        1.0f, //
        2.0f,
        3.0f,
        4.0f,
    }};

    const auto ident = game::Mat4{};

    ASSERT_EQ(m1 * ident, m1);
}

TEST(Matrix4, LookAt)
{
    const auto view = game::Mat4::LookAt(
        {1.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

    const auto expected = game::Mat4{
        {0.980581f,
         0.000000f,
         0.196116f,
         0.000000f,
         0.000000f,
         1.000000f,
         -0.000000f,
         0.000000f,
         -0.196116f,
         0.000000f,
         0.980581f,
         0.000000f,
         -0.000000f,
         -0.000000f,
         -5.099020f,
         1.000000f}};

    auto view_spn = view.Data();
    auto expected_spn = expected.Data();

    for (int i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(view_spn[i], expected_spn[i], 0.001);
    }
}

TEST(Matrix4, Perspective)
{
    auto proj = game::Mat4::Perspective(std::numbers::pi_v<float> / 4.0f, 800.0f, 600.0f, 0.1f, 100.0f);
    auto proj_spn = proj.Data();

    auto expected = game::Mat4{
        {1.810660f,
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         2.414213f,
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         -1.002002f,
         -1.000000f,
         0.0f,
         0.0f,
         -0.200200f,
         0.0f}};

    auto expected_spn = expected.Data();

    for (int i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(proj_spn[i], expected_spn[i], 0.001);
    }
}
