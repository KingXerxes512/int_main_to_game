#include "Renderer.h"
#include "BufferWriter.h"
#include "Camera.h"
#include "Color.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Sampler.h"
#include "Texture.h"

namespace
{

#pragma warning(push)
#pragma warning(disable : 4324)
struct PointLightBuffer
{
    alignas(16) game::Vector3 position;
    alignas(16) game::Color color;
    alignas(16) game::Vector3 attenuation;
};

struct LightBuffer
{
    alignas(16) game::Color ambient;
    alignas(16) game::Vector3 direction;
    alignas(16) game::Color direction_color;
    int numLights;
};
#pragma warning(pop)

}

namespace game
{

Renderer::Renderer()
    : m_CameraBuffer(sizeof(Matrix4) * 2u + sizeof(Vector3))
    , m_LightBuffer(10240u)
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
            .numLights = static_cast<int>(scene.points.size())};

        BufferWriter writer{m_LightBuffer};
        writer.Write(light_buffer);

        for (const auto& point : scene.points)
        {
            PointLightBuffer pointLightBuffer = {
                .position = point.position,
                .color = point.color,
                .attenuation = {point.const_attenuation, point.linear_attenuation, point.quad_attenuation}};

            writer.Write(pointLightBuffer);
        }
    }

    ::glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_LightBuffer.Native_Handle());

    for (const auto* entity : scene.entities)
    {
        const auto* material = entity->Material();
        const auto* mesh = entity->Mesh();

        material->Use();
        material->SetUniform("model", entity->Model());
        material->BindTextures(entity->Textures());

        mesh->Bind();

        ::glDrawElements(
            GL_TRIANGLES, mesh->IndexCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(mesh->IndexOffset()));
        mesh->Unbind();
    }
}
}
