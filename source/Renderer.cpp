#include "Renderer.h"
#include "BufferWriter.h"
#include "Camera.h"
#include "Color.h"
#include "Entity.h"

namespace
{

#pragma warning(push)
#pragma warning(disable : 4324)
struct LightBuffer
{
    alignas(16) game::Color ambient;
    alignas(16) game::Vector3 direction;
    alignas(16) game::Color direction_color;
    alignas(16) game::Vector3 point;
    alignas(16) game::Color point_color;
};
#pragma warning(pop)

}

namespace game
{

Renderer::Renderer()
    : m_CameraBuffer(sizeof(Mat4) * 2u + sizeof(Vector3))
    , m_LightBuffer(sizeof(LightBuffer))
{
}

void Renderer::Render(const Camera& camera, const Scene& scene) const
{
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        BufferWriter writer{m_CameraBuffer};
        writer.Write(camera.View());
        writer.Write(camera.Projection());
        writer.Write(camera.Position());
    }

    ::glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraBuffer.Native_Handle());

    {
        LightBuffer light_buffer{
            .ambient = scene.ambient,
            .direction = scene.directional.direction,
            .direction_color = scene.directional.color,
            .point = scene.point.position,
            .point_color = scene.point.color};

        BufferWriter writer{m_LightBuffer};
        writer.Write(light_buffer);
    }

    ::glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_LightBuffer.Native_Handle());

    for (const auto* entity : scene.entities)
    {
        const auto* material = entity->Material();
        const auto* mesh = entity->Mesh();
        const auto* sampler = entity->Sampler();

        ::glUseProgram(material->Native_Handle());

        const GLint model_uniform = ::glGetUniformLocation(material->Native_Handle(), "model");
        ::glUniformMatrix4fv(model_uniform, 1, GL_FALSE, entity->Model().data());

        for (const auto& [index, tex] : entity->Textures() | std::views::enumerate)
        {
            const ::GLuint idx = static_cast<::GLuint>(index);
            ::glBindTextureUnit(idx, tex->Native_Handle());
            ::glBindSampler(idx, sampler->Native_Handle());

            const auto uniformName = std::format("tex{}", index);

            const GLint texUniform = ::glGetUniformLocation(material->Native_Handle(), uniformName.c_str());
            ::glUniform1i(texUniform, static_cast<::GLint>(index));
        }

        mesh->Bind();

        ::glDrawElements(
            GL_TRIANGLES, mesh->IndexCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(mesh->IndexOffset()));
        mesh->Unbind();
    }
}

}
