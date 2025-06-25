#include "ModelLoader.h"

#include <ranges>

namespace
{

template <class... Args>
constexpr std::vector<game::VertexData> vertices(Args&&... args)
{
    return std::views::zip_transform(
               []<class... A>(A&&... a) constexpr { return game::VertexData{std::forward<A>(a)...}; },
               std::forward<Args>(args)...) |
           std::ranges::to<std::vector>();
}

}

namespace game
{

ModelData ModelLoader::Cube()
{
    auto cube = m_LoadedModels.find("cube");
    if (cube != std::ranges::cend(m_LoadedModels))
    {
        return {.vertices = cube->second.vertices, .indices = cube->second.indices};
    }

    constexpr Vector3 positions[] = {{-1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
                                     {-1.0f, 1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
                                     {1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f},
                                     {-1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},
                                     {1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
                                     {1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},
                                     {1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, -1.0f},
                                     {1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f}};

    constexpr Vector3 normals[] = {{0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, 1.0f},
                                   {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
                                   {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
                                   {1.0f, 0.0f, 0.0f},  {1.0f, 0.0f, 0.0f},  {1.0f, 0.0f, 0.0f},  {1.0f, 0.0f, 0.0f},
                                   {0.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 0.0f},  {0.0f, 1.0f, 0.0f},
                                   {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}};

    constexpr UV uvs[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f},
                          {0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
                          {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
                          {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    const std::vector<uint32_t> indices = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                           12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

    const auto newItem =
        m_LoadedModels.emplace("cube", LoadedModelData{vertices(positions, normals, uvs), std::move(indices)});

    return {.vertices = newItem.first->second.vertices, .indices = newItem.first->second.indices};
}

}
