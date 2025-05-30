#pragma once

#include "AutoRelease.h"
#include "Opengl.h"

#include <cstddef>
#include <cstdint>
#include <span>

namespace game
{

class Buffer
{
  public:
    Buffer(std::uint32_t size);

    void Write(std::span<const std::byte> data, std::size_t offset) const;

    ::GLuint Native_Handle() const;

  private:
    AutoRelease<::GLuint> m_Buffer;
};

}
