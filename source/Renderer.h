#pragma once

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
    void Render() const;

  private:
    Mesh m_Mesh;
    Material m_Material;
};

}
