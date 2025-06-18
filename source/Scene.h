#pragma once

#include "Color.h"
#include "Vector3.h"

#include <vector>

namespace game
{

class Entity;

struct DirectionalLight
{
    Vector3 direction;
    Color color;
};

struct PointLight
{
    Vector3 position;
    Color color;
    float const_attenuation;
    float linear_attenuation;
    float quad_attenuation;
};

struct Scene
{
    std::vector<const Entity*> entities;
    Color ambient;
    DirectionalLight directional;
    PointLight point;
};

}
