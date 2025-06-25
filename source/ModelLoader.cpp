#include "ModelLoader.h"
#include "Error.h"
#include "Log.h"

#include "assimp/Importer.hpp"
#include "assimp/Logger.hpp"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

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

ModelLoader::ModelLoader(ResourceLoader& resourceLoader)
    : m_ResourceLoader(resourceLoader)
{
}

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

    const auto newItem = m_LoadedModels.emplace("cube", LoadedModelData{vertices(positions, normals, uvs), indices});

    return {.vertices = newItem.first->second.vertices, .indices = newItem.first->second.indices};
}

ModelData ModelLoader::Load(std::string_view file, std::string_view name)
{
    auto stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
    aiAttachLogStream(&stream);

    const auto modelFileData = m_ResourceLoader.Load_Binary(file);

    // const auto* scene = ::aiImportFileFromMemory(reinterpret_cast<const char*>(modelFileData.data()),
    // static_cast<uint32_t>(modelFileData.size()), aiProcess_Triangulate, NULL);

    ::Assimp::Importer importer{};
    const auto* scene =
        importer.ReadFileFromMemory(modelFileData.data(), modelFileData.size(), ::aiProcess_Triangulate);

    ensure(scene != nullptr && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE), "Failed to load model {} {}", file, name);

    std::span<::aiMesh*> loadedMeshes = {scene->mMeshes, scene->mNumMeshes};
    for (const auto* mesh : loadedMeshes)
    {
        const auto loaded = m_LoadedModels.find(mesh->mName.C_Str());
        if (loaded != std::ranges::cend(m_LoadedModels))
        {
            /*return ModelData{.vertices = loaded->second.vertices, .indices = loaded->second.indices};*/
            continue;
        }

        const auto toVector3 = [](const ::aiVector3D& v) { return Vector3{v.x, v.y, v.z}; };
        const auto toUV = [](const auto*& v) { return UV{.x = v[0].x, .y = v[0].y}; };
        const auto positions =
            std::span<::aiVector3D>{mesh->mVertices, mesh->mNumVertices} | std::views::transform(toVector3);
        const auto normals =
            std::span<::aiVector3D>{mesh->mNormals, mesh->mNumVertices} | std::views::transform(toVector3);

        game::log::debug("{} has texture coords: {}", mesh->mName.C_Str(), mesh->HasTextureCoords(0));

        std::vector<UV> uvs = {};
        for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
        {
            if (mesh->HasTextureCoords(0))
                uvs.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            else
                uvs.emplace_back(UV{.x = 0, .y = 0});
        }

        /*const auto uvs =
            std::span<::aiVector3D*>(mesh->mTextureCoords, mesh->mNumVertices) | std::views::transform(toUV);*/

        std::vector<uint32_t> indices = {};
        for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
        {
            const auto& face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; ++j)
            {
                indices.emplace_back(face.mIndices[j]);
            }
        }

        m_LoadedModels.emplace(
            mesh->mName.C_Str(), LoadedModelData{vertices(positions, normals, uvs), std::move(indices)});
    }

    const auto loaded = m_LoadedModels.find(name);
    if (loaded != std::ranges::cend(m_LoadedModels))
    {
        return ModelData{.vertices = loaded->second.vertices, .indices = loaded->second.indices};
    }
    else
    {
        return {};
    }

    // ensure(false, "Failed to load {} from {}", name, file);
    // std::unreachable();
}

}
