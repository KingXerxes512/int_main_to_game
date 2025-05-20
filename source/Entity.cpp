#include "Entity.h"

namespace game
{

Entity::Entity(const game::Mesh* mesh, const game::Material* material)
    : m_Mesh(mesh)
    , m_Material(material)
    , m_Model()
{
}

std::span<const float> Entity::Model() const
{
    return m_Model.Data();
}

}
