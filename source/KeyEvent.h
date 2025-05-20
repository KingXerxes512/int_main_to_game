#pragma once

#include "Key.h"

namespace game
{

class KeyEvent
{
  public:
    explicit KeyEvent(game::Key key);
    game::Key Key() const
    {
        return m_Key;
    }

  private:
    game::Key m_Key;
};

}
