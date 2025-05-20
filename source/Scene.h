#pragma once

#include <vector>

#include "Entity.h"

namespace game
{

struct Scene
{
    std::vector<const Entity*> m_Entities;
};

}
