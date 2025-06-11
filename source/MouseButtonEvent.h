#pragma once

#include <format>

namespace game
{

enum class MouseButtonState
{
    UP,
    DOWN
};

class MouseButtonEvent
{
  public:
    MouseButtonEvent(float x, float y, MouseButtonState state)
        : m_X(x)
        , m_Y(y)
        , m_State(state)
    {
    }

    float X() const
    {
        return m_X;
    }
    float Y() const
    {
        return m_Y;
    }
    MouseButtonState State() const
    {
        return m_State;
    }

  private:
    float m_X;
    float m_Y;
    MouseButtonState m_State;
};

}

// template <>
// struct std::formatter<game::MouseButtonEvent>
//{
//     constexpr auto parse(std::format_parse_context& ctx)
//     {
//         return std::begin(ctx);
//     }
//
//     auto format(const game::MouseButtonEvent& obj, std::format_context& ctx) const
//     {
//         return std::format_to(ctx.out(), "MouseEvent {} {}", obj.DeltaX(), obj.DeltaY());
//     }
// };
