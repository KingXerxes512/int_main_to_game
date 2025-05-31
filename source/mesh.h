#pragma once

#include "AutoRelease.h"
#include "Buffer.h"
#include "Opengl.h"
#include "Log.h"

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
    Buffer m_VBO;
    std::uint32_t m_IndexCount;
    std::uintptr_t m_IndexOffset;
};

}
