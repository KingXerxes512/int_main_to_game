#include "Buffer.h"
#include "Error.h"

namespace game
{

Buffer::Buffer(std::uint32_t size)
    : m_Buffer{0u, [](auto buffer) { ::glDeleteBuffers(1, &buffer); }}
{
    ::glCreateBuffers(1, &m_Buffer);
    ensure(m_Buffer, "Failed to create buffer!");
    ::glNamedBufferStorage(m_Buffer, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void Buffer::Write(std::span<const std::byte> data, std::size_t offset) const
{
    ::glNamedBufferSubData(m_Buffer, offset, data.size(), data.data());
}

::GLuint Buffer::Native_Handle() const
{
    return m_Buffer;
}

}
