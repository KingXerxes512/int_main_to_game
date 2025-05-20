#include "Entity.h"

namespace game
{

Entity::Entity(const game::Mesh* mesh, const game::Material* material, const game::Vector3& position)
    : m_Mesh(mesh)
    , m_Material(material)
    , m_Model(Mat4(position))
{
}

std::span<const float> Entity::Model() const
{
    return m_Model.Data();
}

}
