#pragma once

#include "AutoRelease.h"
#include "Opengl.h"

namespace game
{

class Sampler
{
  public:
    Sampler();

    ::GLuint Native_Handle() const;

  private:
    AutoRelease<::GLuint> m_Handle;
};

}
