#include "material.h"
#include "error.h"

namespace game
{

Material::Material(const Shader& vertex_shader, const Shader& fragment_shader)
    : m_Handle()
{
    ensure(vertex_shader.Type() == ShaderType::VERTEX, "vertex_shader is not a vertex shader!");
    ensure(fragment_shader.Type() == ShaderType::FRAGMENT, "fragment_shader is not a fragment shader!");

    m_Handle = game::AutoRelease<::GLuint>(::glCreateProgram(), ::glDeleteProgram);
    ensure(m_Handle, "Failed to create opengl program!");

    ::glAttachShader(m_Handle, vertex_shader.Native_Handle());
    ::glAttachShader(m_Handle, fragment_shader.Native_Handle());
    ::glLinkProgram(m_Handle);
}

::GLuint Material::Native_Handle() const
{
    return m_Handle;
}

}
