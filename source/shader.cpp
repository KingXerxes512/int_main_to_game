#include "shader.h"
#include "error.h"

namespace
{
::GLenum to_native(const game::ShaderType type)
{
    switch (type)
    {
        using enum game::ShaderType;
        case VERTEX: return GL_VERTEX_SHADER;
        case FRAGMENT: return GL_FRAGMENT_SHADER;
    }

    throw game::Exception("Unknown shader type: {}", std::to_underlying(type));
}
}

namespace game
{

Shader::Shader(std::string_view source, ShaderType type)
    : m_Handle()
    , m_Type(type)
{
    m_Handle = AutoRelease<::GLuint>(::glCreateShader(to_native(type)), ::glDeleteShader);

    const ::GLchar* strings = {source.data()};
    const ::GLint lengths[] = {static_cast<::GLint>(source.length())};

    ::glShaderSource(m_Handle, 1, &strings, lengths);
    ::glCompileShader(m_Handle);

    ::GLint result;
    ::glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &result);
    ensure(result, "Failed to compile shader {}", type);
}

ShaderType Shader::Type() const
{
    return m_Type;
}

::GLuint Shader::Native_Handle() const
{
    return m_Handle;
}

}
