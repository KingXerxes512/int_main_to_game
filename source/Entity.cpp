#include <vector>

#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Sampler.h"

namespace game
{

Entity::Entity(
    const game::Mesh* mesh,
    const game::Material* material,
    const game::Vector3& position,
    const game::Vector3& scale,
    const std::vector<std::tuple<const game::Texture*, const game::Sampler*>>& textures)
    : m_Mesh(mesh)
    , m_Material(material)
    , m_Model(Matrix4(position, scale))
    , m_Textures(textures)
{
}

const game::Mesh* Entity::Mesh() const
{
    return m_Mesh;
}

const game::Material* Entity::Material() const
{
    return m_Material;
}

const game::Matrix4& Entity::Model() const
{
    return m_Model;
}

std::span<const std::tuple<const game::Texture*, const game::Sampler*>> Entity::Textures() const
{
    return m_Textures;
}

}
