#pragma once

#include "StringMap.h"
#include "VertexData.h"

#include <cstdint>
#include <span>
#include <vector>

namespace game
{

struct ModelData
{
    std::span<const VertexData> vertices;
    std::span<const uint32_t> indices;
};

class ModelLoader
{
  public:
    ModelData Cube();
  private:
    struct LoadedModelData
    {
        std::vector<VertexData> vertices;
        std::vector<uint32_t> indices;
    };

    StringMap<LoadedModelData> m_LoadedModels;
};

}
