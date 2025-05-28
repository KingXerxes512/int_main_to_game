#pragma once

#include <variant>

#include "StopEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace game
{

using Event = std::variant<StopEvent, KeyEvent, MouseEvent>;

}

