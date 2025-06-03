#include "Renderer.h"
#include "BufferWriter.h"
#include "Camera.h"

namespace game
{

Renderer::Renderer()
    : m_CameraBuffer(sizeof(Mat4) * 2u)
{
}

void Renderer::Render(const Camera& camera, const Scene& scene) const
{
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        BufferWriter writer{m_CameraBuffer};
        writer.Write(camera.View());
        writer.Write(camera.Projection());
    }

    ::glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraBuffer.Native_Handle());

    for (const auto* entity : scene.m_Entities)
    {
        const auto* material = entity->Material();
        const auto* mesh = entity->Mesh();
        const auto* texture = entity->Texture();
        const auto* sampler = entity->Sampler();
        ::glUseProgram(material->Native_Handle());

        const GLint model_uniform = ::glGetUniformLocation(material->Native_Handle(), "model");
        ::glUniformMatrix4fv(model_uniform, 1, GL_FALSE, entity->Model().data());

        ::glBindTextureUnit(0, texture->Native_Handle());
        ::glBindSampler(0, sampler->Native_Handle());

        const GLint texUniform = ::glGetUniformLocation(material->Native_Handle(), "tex");
        ::glUniform1i(texUniform, 0);

        mesh->Bind();

        ::glDrawElements(
            GL_TRIANGLES, mesh->IndexCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(mesh->IndexOffset()));
        mesh->Unbind();
    }
}

}
