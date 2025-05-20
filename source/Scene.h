#pragma once

#include <vector>

#include "Entity.h"

namespace game
{

class Scene
{
  public:
  //private:
    std::vector<const Entity*> m_Entities;
};

}
