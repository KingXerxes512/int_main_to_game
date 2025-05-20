#pragma once

#include "Camera.h"
#include "AutoRelease.h"
#include "Scene.h"
#include "Opengl.h"

namespace game
{

class Renderer
{
  public:
    void Render(const Camera& camera, const Scene& scene) const;

  private:
};

}
