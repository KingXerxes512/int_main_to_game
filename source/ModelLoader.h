#pragma once

#include "ResourceLoader.h"
#include "StringMap.h"
#include "VertexData.h"

#include <cstdint>
#include <span>
#include <string_view>
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
    ModelLoader(ResourceLoader& resourceLoader);

    ModelData Cube();
    ModelData Load(std::string_view file, std::string_view name);

  private:
    struct LoadedModelData
    {
        std::vector<VertexData> vertices;
        std::vector<uint32_t> indices;
    };

    StringMap<LoadedModelData> m_LoadedModels;
    ResourceLoader& m_ResourceLoader;
};

}
