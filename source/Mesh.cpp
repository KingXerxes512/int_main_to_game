#include "Mesh.h"
#include "Buffer.h"
#include "BufferWriter.h"
#include "MeshLoader.h"
#include "VertexData.h"

namespace game
{

Mesh::Mesh(const MeshData& data)
    : m_VAO(0u, [](auto vao) { ::glDeleteVertexArrays(1, &vao); })
    , m_VBO{static_cast<uint32_t>(data.vertices.size_bytes() + data.indices.size_bytes())}
    , m_IndexCount(static_cast<std::uint32_t>(data.indices.size()))
    , m_IndexOffset(data.vertices.size_bytes())
{
    {
        BufferWriter writer{m_VBO};
        writer.Write(data.vertices);
        writer.Write(data.indices);
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
