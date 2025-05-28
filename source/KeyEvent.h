#pragma once

#include "Key.h"

#include <format>

namespace game
{

class KeyEvent
{
  public:
    explicit KeyEvent(game::Key key, game::KeyState state);
    game::Key Key() const
    {
        return m_Key;
    }

    game::KeyState State() const
    {
        return m_KeyState;
    }

  private:
    game::Key m_Key;
    game::KeyState m_KeyState;
};

}

template <>
struct std::formatter<game::KeyEvent>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::begin(ctx);
    }

    auto format(const game::KeyEvent& obj, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "Key={} State={}", obj.Key(), obj.State());
    }
};
