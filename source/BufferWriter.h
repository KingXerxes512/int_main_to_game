#pragma once

#include "Buffer.h"

#include <span>

namespace game
{

class BufferWriter
{
  public:
    BufferWriter(const Buffer& buffer);

    template <class T>
    void Write(const T& obj) requires (std::is_trivially_copyable_v<T>)
    {
        auto spn = std::span<const T>{&obj, 1};
        Write(spn);
    }

    template <class T, std::size_t N>
    void Write(const T (&data)[N])
    {
        m_Buffer.Write({reinterpret_cast<const std::byte*>(data), sizeof(T) * N}, m_Offset);
        m_Offset += sizeof(T) * N;
    }

    // I want this to work so badly
    template <class T>
    void Write(std::span<const T> data)
    {
        m_Buffer.Write(std::as_bytes(data), m_Offset);
        m_Offset += data.size_bytes();
    }

  private:
    const Buffer& m_Buffer;
    std::size_t m_Offset;
};

}
