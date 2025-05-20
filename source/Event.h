#pragma once

#include <variant>

#include "StopEvent.h"
#include "KeyEvent.h"

namespace game
{

using Event = std::variant<StopEvent, KeyEvent>;

}

