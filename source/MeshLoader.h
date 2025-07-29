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

struct MeshData
{
    std::span<const VertexData> vertices;
    std::span<const uint32_t> indices;
};

class MeshLoader
{
  public:
    MeshLoader(ResourceLoader& resourceLoader);

    MeshData Cube();
    MeshData Load(std::string_view meshFile, std::string_view meshName);

  private:
    struct LoadedModelData
    {
        std::vector<VertexData> vertices;
        std::vector<uint32_t> indices;
    };

    StringMap<LoadedModelData> m_LoadedMeshes;
    ResourceLoader& m_ResourceLoader;
};

}
