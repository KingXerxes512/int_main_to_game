#include "MouseEvent.h"

namespace game
{

MouseEvent::MouseEvent(float deltaX, float deltaY)
    : m_DeltaX(deltaX)
    , m_DeltaY(deltaY)
{
}

float MouseEvent::DeltaX() const
{
    return m_DeltaX;
}

float MouseEvent::DeltaY() const
{
    return m_DeltaY;
}

}
