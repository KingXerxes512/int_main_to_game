#pragma once

#include "Color.h"
#include "Vector3.h"

namespace game
{

struct UV
{
    float x, y;
};

struct VertexData
{
    Vector3 position;
    Vector3 normal;
    UV uv;
};

}
