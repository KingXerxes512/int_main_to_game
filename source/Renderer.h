#pragma once

#include "Camera.h"
#include "AutoRelease.h"
#include "Material.h"
#include "Mesh.h"
#include "Opengl.h"

namespace game
{

class Renderer
{
  public:
    Renderer(Material material);
    void Render(const Camera& camera) const;

  private:
    Mesh m_Mesh;
    Material m_Material;
};

}
