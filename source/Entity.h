#pragma once

#include <span>

#include "Material.h"
#include "Matrix4.h"
#include "Mesh.h"
#include "Sampler.h"
#include "Texture.h"
#include "Vector3.h"

namespace game
{

class Entity
{
  public:
    Entity(
        const game::Mesh* mesh,
        const game::Material* material,
        const game::Vector3& position,
        const std::vector<const game::Texture*>& textures,
        const game::Sampler* sampler);

    const game::Mesh* Mesh() const
    {
        return m_Mesh;
    }

    const game::Material* Material() const
    {
        return m_Material;
    }

    std::span<const float> Model() const;

    const std::vector<const game::Texture*>& Textures() const;
    const Sampler* Sampler() const;

  private:
    const game::Mesh* m_Mesh;
    const game::Material* m_Material;
    Mat4 m_Model;
    std::vector<const game::Texture*> m_Textures;
    const game::Sampler* m_Sampler;
};

}
