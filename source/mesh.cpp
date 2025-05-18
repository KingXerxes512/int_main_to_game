#include "Mesh.h"
#include "VertexData.h"

namespace
{

// clang-format off
static constexpr game::VertexData vertex_data[] = {
    {.position = {.x = 0.0f,  .y = 0.5f,  .z = 0.0f}, .color = {.r = 1.0f, .g = 0.0f, .b = 0.0f}},
    {.position = {.x = -0.5f, .y = -0.5f, .z = 0.0f}, .color = {.r = 0.0f, .g = 1.0f, .b = 0.0f}},
    {.position = {.x = 0.5f,  .y = -0.5f, .z = 0.0f}, .color = {.r = 0.0f, .g = 0.5f, .b = 1.0f}}
};
// clang-format on

}

namespace game
{

Mesh::Mesh()
    : m_VAO(0u, [](auto vao) { ::glDeleteVertexArrays(1, &vao); })
    , m_VBO(0u, [](auto vbo) { ::glDeleteBuffers(1, &vbo); })
{
    ::glGenVertexArrays(1, &m_VAO);

    ::glGenBuffers(1, &m_VBO);
    Bind();

    ::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    ::glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    ::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    ::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    ::glEnableVertexAttribArray(0);
    ::glEnableVertexAttribArray(1);

    Unbind();
}

void Mesh::Bind() const
{
    ::glBindVertexArray(m_VAO);
}

void Mesh::Unbind() const
{
    ::glBindVertexArray(0);
}

::GLuint Mesh::VAO() const
{
    return m_VAO;
}

::GLuint Mesh::VBO() const
{
    return m_VBO;
}

}
