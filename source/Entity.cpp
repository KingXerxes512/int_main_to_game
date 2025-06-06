#include "Entity.h"

#include <vector>

namespace game
{

Entity::Entity(
    const game::Mesh* mesh,
    const game::Material* material,
    const game::Vector3& position,
    const std::vector<const game::Texture*>& textures,
    const game::Sampler* sampler)
    : m_Mesh(mesh)
    , m_Material(material)
    , m_Model(Mat4(position))
    , m_Textures(textures)
    , m_Sampler(sampler)
{
}

std::span<const float> Entity::Model() const
{
    return m_Model.Data();
}

const std::vector<const game::Texture*>& Entity::Textures() const
{
    return m_Textures;
}

const game::Sampler* Entity::Sampler() const
{
    return m_Sampler;
}

}
