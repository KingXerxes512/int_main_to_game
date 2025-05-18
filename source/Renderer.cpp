#include "Renderer.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Log.h"

#include <numbers>

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
    ::glClear(GL_DEPTH_BUFFER_BIT);

    ::glUseProgram(m_Material.Native_Handle());

    static auto x = 2.0f;
    static auto y = 2.0f;
    static auto z = 0.0f;
    static auto t = 0.0f;

    x = std::sin(t) * 5.0f;
    y = std::cos(t) * 5.0f;
    z = std::cos(t) * 5.0f;

    t += 0.01f;

    const auto model = Mat4(Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f});
    const GLint model_uniform = ::glGetUniformLocation(m_Material.Native_Handle(), "model");
    ::glUniformMatrix4fv(model_uniform, 1, GL_FALSE, model.Data().data());

    const auto view = Mat4::LookAt(
        {.x = x, .y = 3.0f, .z = z}, {.x = 0.0f, .y = 0.0f, .z = 0.0f}, {.x = 0.0f, .y = 1.0f, .z = 0.0f});
    const GLint view_uniform = ::glGetUniformLocation(m_Material.Native_Handle(), "view");
    ::glUniformMatrix4fv(view_uniform, 1, GL_FALSE, view.Data().data());

    static const auto proj = Mat4::Perspective(std::numbers::pi_v<float> / 4.0f, 800.0f, 600.0f, 0.001f, 100.0f);
    const GLint proj_uniform = ::glGetUniformLocation(m_Material.Native_Handle(), "projection");
    ::glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, proj.Data().data());

    m_Mesh.Bind();
    ::glDrawArrays(GL_TRIANGLES, 0, 36);
    m_Mesh.Unbind();
}

}
