#include "Mesh.h"
#include "Buffer.h"
#include "BufferWriter.h"
#include "VertexData.h"

namespace
{

constexpr game::VertexData vertex_data[] = {
    {.position = {-1.0f, -1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {0.0f, 0.0f}}, //
    {.position = {1.0f, -1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {1.0f, 0.0f}},  //
    {.position = {1.0f, 1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {1.0f, 1.0f}},   //
    {.position = {-1.0f, 1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {0.0f, 1.0f}},  //

    {.position = {-1.0f, -1.0f, -1.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 0.0f}}, //
    {.position = {1.0f, -1.0f, -1.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 0.0f}},  //
    {.position = {1.0f, 1.0f, -1.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 1.0f}},   //
    {.position = {-1.0f, 1.0f, -1.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 1.0f}},  //

    {.position = {-1.0f, -1.0f, -1.0f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {0.0f, 0.0f}}, //
    {.position = {-1.0f, -1.0f, 1.0f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {1.0f, 0.0f}},  //
    {.position = {-1.0f, 1.0f, 1.0f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {1.0f, 1.0f}},   //
    {.position = {-1.0f, 1.0f, -1.0f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {0.0f, 1.0f}},  //

    {.position = {1.0f, -1.0f, -1.0f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {0.0f, 0.0f}}, //
    {.position = {1.0f, -1.0f, 1.0f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {1.0f, 0.0f}},  //
    {.position = {1.0f, 1.0f, 1.0f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {1.0f, 1.0f}},   //
    {.position = {1.0f, 1.0f, -1.0f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {0.0f, 1.0f}},  //

    {.position = {-1.0f, 1.0f, -1.0f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {0.0f, 0.0f}}, //
    {.position = {1.0f, 1.0f, -1.0f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {1.0f, 0.0f}},  //
    {.position = {1.0f, 1.0f, 1.0f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {1.0f, 1.0f}},   //
    {.position = {-1.0f, 1.0f, 1.0f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {0.0f, 1.0f}},  //

    {.position = {-1.0f, -1.0f, -1.0f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {0.0f, 0.0f}}, //
    {.position = {1.0f, -1.0f, -1.0f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {1.0f, 0.0f}},  //
    {.position = {1.0f, -1.0f, 1.0f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {1.0f, 1.0f}},   //
    {.position = {-1.0f, -1.0f, 1.0f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {0.0f, 1.0f}},  //
};

constexpr ::GLuint indices[] = {
    0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8, //
    12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20 //
};

}

namespace game
{

Mesh::Mesh()
    : m_VAO(0u, [](auto vao) { ::glDeleteVertexArrays(1, &vao); })
    , m_VBO{sizeof(vertex_data) + sizeof(indices)}
    , m_IndexCount(static_cast<std::uint32_t>(std::ranges::distance(indices)))
    , m_IndexOffset(sizeof(vertex_data))
{
    {
        BufferWriter writer{m_VBO};
        writer.Write(vertex_data);
        writer.Write(indices);
    }

    ::glCreateVertexArrays(1, &m_VAO);

    ::glVertexArrayVertexBuffer(m_VAO, 0, m_VBO.Native_Handle(), 0, sizeof(VertexData));
    ::glVertexArrayElementBuffer(m_VAO, m_VBO.Native_Handle());

    ::glEnableVertexArrayAttrib(m_VAO, 0); // enable position
    ::glEnableVertexArrayAttrib(m_VAO, 1); // enable normal
    ::glEnableVertexArrayAttrib(m_VAO, 2); // enable uv

    ::glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, position));
    ::glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, normal));
    ::glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, uv));

    ::glVertexArrayAttribBinding(m_VAO, 0, 0);
    ::glVertexArrayAttribBinding(m_VAO, 1, 0);
    ::glVertexArrayAttribBinding(m_VAO, 2, 0);

    ////////////
#if OLD
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
#endif
}

void Mesh::Bind() const
{
    ::glBindVertexArray(m_VAO);
}

void Mesh::Unbind() const
{
    ::glBindVertexArray(0);
}

std::uint32_t Mesh::IndexCount() const
{
    return m_IndexCount;
}

std::uintptr_t Mesh::IndexOffset() const
{
    return m_IndexOffset;
}

::GLuint Mesh::VAO() const
{
    return m_VAO;
}

::GLuint Mesh::VBO() const
{
    return m_VBO.Native_Handle();
}

}
