#pragma once

#include "Entity.h"
#include "Color.h"

#include <vector>

namespace game
{

struct Scene
{
    std::vector<const Entity*> entities;
    Color ambient;
};

}
