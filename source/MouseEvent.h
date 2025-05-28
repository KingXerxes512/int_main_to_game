#pragma once

#include <format>

namespace game
{

class MouseEvent
{
  public:
    MouseEvent(float deltaX, float deltaY);

    float DeltaX() const;
    float DeltaY() const;

  private:
    float m_DeltaX;
    float m_DeltaY;
};

}

template <>
struct std::formatter<game::MouseEvent>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::begin(ctx);
    }

    auto format(const game::MouseEvent& obj, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "MouseEvent {} {}", obj.DeltaX(), obj.DeltaY());
    }
};
