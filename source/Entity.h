#pragma once

#include <span>

#include "Material.h"
#include "Matrix4.h"
#include "Mesh.h"

namespace game
{

class Entity
{
  public:
    Entity(const game::Mesh* mesh, const game::Material* material);

    const game::Mesh* Mesh() const
    {
        return m_Mesh;
    }

    const game::Material* Material() const
    {
        return m_Material;
    }

    std::span<const float> Model() const;

  private:
    const game::Mesh* m_Mesh;
    const game::Material* m_Material;
    Mat4 m_Model;
};

}
