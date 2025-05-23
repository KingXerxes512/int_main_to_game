#include "KeyEvent.h"

namespace game
{

KeyEvent::KeyEvent(game::Key key, game::KeyState state)
    : m_Key(key)
    , m_KeyState(state)
{
}

}
