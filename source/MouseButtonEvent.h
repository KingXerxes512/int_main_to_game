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
    MouseButtonEvent(float x, float y, MouseButtonState state);
    float X() const;
    float Y() const;
    MouseButtonState State() const;

  private:
    float m_X;
    float m_Y;
    MouseButtonState m_State;
};

}

template <>
struct std::formatter<game::MouseButtonEvent>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::begin(ctx);
    }

    auto format(const game::MouseButtonEvent& obj, std::format_context& ctx) const
    {
        switch (obj.State())
        {
            using enum game::MouseButtonState;
            case UP: return std::format_to(ctx.out(), "UP");
            case DOWN: return std::format_to(ctx.out(), "DOWN");
            default: return std::format_to(ctx.out(), "UNKNOWN");
        }
    }
};
