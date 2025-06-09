#pragma once

#include "AutoRelease.h"
#include "Matrix4.h"
#include "Opengl.h"
#include "Shader.h"
#include "StringMap.h"

#include <string_view>
#include <cstdint>

namespace game
{

class Material
{
  public:
    Material(const Shader& vertex_shader, const Shader& fragment_shader);

    void Use() const;
    void SetUniform(std::string_view name, const Matrix4& obj) const;
    void BindTexture(std::uint32_t index, const Texture* texture, const Sampler* sampler) const;

    ::GLuint Native_Handle() const;

  private:
    AutoRelease<::GLuint> m_Handle;
    StringMap<::GLuint> m_Uniforms;
};

}
