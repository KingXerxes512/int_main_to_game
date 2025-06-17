#include "MouseButtonEvent.h"

namespace game
{

MouseButtonEvent::MouseButtonEvent(float x, float y, MouseButtonState state)
    : m_X(x)
    , m_Y(y)
    , m_State(state)
{
}

float MouseButtonEvent::X() const
{
    return m_X;
}

float MouseButtonEvent::Y() const
{
    return m_Y;
}

MouseButtonState MouseButtonEvent::State() const
{
    return m_State;
}

}
