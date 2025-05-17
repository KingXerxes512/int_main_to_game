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

    ::GLuint VAO() const;
    ::GLuint VBO() const;

  private:
    AutoRelease<::GLuint> m_VAO;
    AutoRelease<::GLuint> m_VBO;
};

}
