#pragma once

#include "AutoRelease.h"
#include "Opengl.h"

#include <cstddef>
#include <cstdint>
#include <span>

namespace game
{

class Texture
{
  public:
    Texture(std::span<const std::byte> data, std::uint32_t width, std::uint32_t height);

    ::GLuint Native_Handle() const;

  private:
    AutoRelease<::GLuint> m_Handle;
};

}
