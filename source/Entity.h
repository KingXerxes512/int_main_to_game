#pragma once

#include "Matrix4.h"
#include "Vector3.h"

#include <tuple>
#include <vector>

namespace game
{

class Material;
class Mesh;
class Texture;
class Sampler;

class Entity
{
  public:
    Entity(
        const game::Mesh* mesh,
        const game::Material* material,
        const game::Vector3& position,
        const std::vector<std::tuple<const Texture*, const Sampler*>>& textures);

    const game::Mesh* Mesh() const;
    const game::Material* Material() const;
    const Matrix4& Model() const;

    std::span<const std::tuple<const game::Texture*, const game::Sampler*>> Textures() const;

  private:
    const game::Mesh* m_Mesh;
    const game::Material* m_Material;
    Matrix4 m_Model;
    std::vector<std::tuple<const game::Texture*, const game::Sampler*>> m_Textures;
};

}
