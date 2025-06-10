#include "Material.h"
#include "Error.h"
#include "Log.h"
#include "Opengl.h"

namespace game
{

Material::Material(const Shader& vertex_shader, const Shader& fragment_shader)
    : m_Handle()
    , m_Uniforms{}
{
    ensure(vertex_shader.Type() == ShaderType::VERTEX, "vertex_shader is not a vertex shader!");
    ensure(fragment_shader.Type() == ShaderType::FRAGMENT, "fragment_shader is not a fragment shader!");

    m_Handle = game::AutoRelease<::GLuint>(::glCreateProgram(), ::glDeleteProgram);
    ensure(m_Handle, "Failed to create opengl program!");

    ::glAttachShader(m_Handle, vertex_shader.Native_Handle());
    ::glAttachShader(m_Handle, fragment_shader.Native_Handle());
    ::glLinkProgram(m_Handle);

    ::GLint result = GL_FALSE;
    ::glGetProgramiv(m_Handle, GL_LINK_STATUS, &result);

    if (!result)
    {
        char errorLog[512] = {0};
        ::glGetProgramInfoLog(m_Handle, 512, nullptr, errorLog);
        ensure(result == GL_TRUE, "Program failed to link! Reason: {}", errorLog);
    }

    ::GLint uniformCount = 0;
    ::glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &uniformCount);

    if (uniformCount != 0)
    {
        ::GLint maxNameLength = 0;
        ::glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
        log::debug("max name length: {}", maxNameLength);

        ::GLsizei length = 0;
        ::GLsizei count = 0;
        ::GLenum type = 0;

        for (auto i = 0; i < uniformCount; ++i)
        {
            std::string name("\0", maxNameLength);
            ::glGetActiveUniform(m_Handle, i, maxNameLength, &length, &count, &type, name.data());
            name.resize(length);

            const auto location = ::glGetUniformLocation(m_Handle, name.c_str());

            m_Uniforms[name] = location;

            log::debug("found uniform: {}", name);
        }
    }

    log::info("new material ({} uniforms)", uniformCount);
}

void Material::Use() const
{
    ::glUseProgram(m_Handle);
}

void Material::SetUniform(std::string_view name, const Matrix4& obj) const
{
    const auto uniform = m_Uniforms.find(name);
    ensure(uniform != std::ranges::cend(m_Uniforms), "Missin uniform {}", name);

    ::glUniformMatrix4fv(uniform->second, 1, GL_FALSE, obj.Data().data());
}

void Material::BindTexture(std::uint32_t index, const Texture* texture, const Sampler* sampler) const
{
    USE(&index);
    USE(texture);
    USE(sampler);
}

::GLuint Material::Native_Handle() const
{
    return m_Handle;
}

}
