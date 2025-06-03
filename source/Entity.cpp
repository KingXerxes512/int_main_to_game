#include "Entity.h"

namespace game
{

Entity::Entity(
    const game::Mesh* mesh,
    const game::Material* material,
    const game::Vector3& position,
    const game::Texture* texture,
    const game::Sampler* sampler)
    : m_Mesh(mesh)
    , m_Material(material)
    , m_Model(Mat4(position))
    , m_Texture(texture)
    , m_Sampler(sampler)
{
}

std::span<const float> Entity::Model() const
{
    return m_Model.Data();
}

const game::Texture* Entity::Texture() const
{
    return m_Texture;
}

const game::Sampler* Entity::Sampler() const
{
    return m_Sampler;
}

}
