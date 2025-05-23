#pragma once

#include "Key.h"

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
