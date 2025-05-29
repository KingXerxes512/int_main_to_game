#pragma once

#include "AutoRelease.h"
#include "Opengl.h"

namespace game
{

class Mesh
{
  public:
    Mesh();

    void Bind() const;
    void Unbind() const;

    std::uint32_t IndexCount() const;
    std::uintptr_t IndexOffset() const;

    ::GLuint VAO() const;
    ::GLuint VBO() const;

  private:
    AutoRelease<::GLuint> m_VAO;
    AutoRelease<::GLuint> m_VBO;
    std::uint32_t m_IndexCount;
    std::uintptr_t m_IndexOffset;
};

}
