#include "Renderer.h"
#include "Camera.h"

namespace game
{

void Renderer::Render(const Camera& camera, const Scene& scene) const
{
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto* entity : scene.m_Entities)
    {
        const auto* material = entity->Material();
        const auto* mesh = entity->Mesh();
        ::glUseProgram(material->Native_Handle());

        const GLint model_uniform = ::glGetUniformLocation(material->Native_Handle(), "model");
        ::glUniformMatrix4fv(model_uniform, 1, GL_FALSE, entity->Model().data());

        const GLint view_uniform = ::glGetUniformLocation(material->Native_Handle(), "view");
        ::glUniformMatrix4fv(view_uniform, 1, GL_FALSE, camera.View().data());

        const GLint proj_uniform = ::glGetUniformLocation(material->Native_Handle(), "projection");
        ::glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, camera.Projection().data());

        mesh->Bind();
        ::glDrawElements(
            GL_TRIANGLES, mesh->IndexCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(mesh->IndexOffset()));
        mesh->Unbind();
    }
}

}
