#pragma once

#include "AutoRelease.h"
#include "Opengl.h"
#include "Shader.h"

namespace game
{

class Material
{
  public:
    Material(const Shader& vertex_shader, const Shader& fragment_shader);
    ::GLuint Native_Handle() const;

  private:
    AutoRelease<::GLuint> m_Handle;
};

}
