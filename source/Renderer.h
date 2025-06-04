#pragma once

#include "AutoRelease.h"
#include "Buffer.h"
#include "Camera.h"
#include "Log.h"
#include "Opengl.h"
#include "Scene.h"

namespace game
{

class Renderer
{
  public:
    Renderer();

    void Render(const Camera& camera, const Scene& scene) const;

  private:
    Buffer m_CameraBuffer;
    Buffer m_LightBuffer;
};

}
