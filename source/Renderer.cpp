#include "Renderer.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Log.h"
#include "Camera.h"

#include <numbers>

namespace game
{

Renderer::Renderer(Material material)
    : m_Mesh()
    , m_Material(std::move(material))
{
    ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::Render(const Camera& camera) const
{
    ::glClear(GL_COLOR_BUFFER_BIT);
    ::glClear(GL_DEPTH_BUFFER_BIT);

    ::glUseProgram(m_Material.Native_Handle());

    const auto model = Mat4(Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f});
    const GLint model_uniform = ::glGetUniformLocation(m_Material.Native_Handle(), "model");
    ::glUniformMatrix4fv(model_uniform, 1, GL_FALSE, model.Data().data());

    const GLint view_uniform = ::glGetUniformLocation(m_Material.Native_Handle(), "view");
    ::glUniformMatrix4fv(view_uniform, 1, GL_FALSE, camera.View().data());

    const GLint proj_uniform = ::glGetUniformLocation(m_Material.Native_Handle(), "projection");
    ::glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, camera.Projection().data());

    m_Mesh.Bind();
    ::glDrawArrays(GL_TRIANGLES, 0, 36);
    m_Mesh.Unbind();
}

}
