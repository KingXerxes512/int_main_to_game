#include "Renderer.h"

namespace game
{

Renderer::Renderer(Material material)
    : m_Mesh()
    , m_Material(std::move(material))
{
    ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::Render() const
{
    ::glClear(GL_COLOR_BUFFER_BIT);

    ::glUseProgram(m_Material.Native_Handle());
    m_Mesh.Bind();
    ::glDrawArrays(GL_TRIANGLES, 0, 3);
    m_Mesh.Unbind();
}

}
