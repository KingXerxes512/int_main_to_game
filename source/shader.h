#pragma once

#include <format>
#include <string_view>

#include "AutoRelease.h"
#include "Exception.h"
#include "Opengl.h"

namespace game
{

enum class ShaderType
{
    VERTEX,
    FRAGMENT
};

class Shader
{
  public:
    Shader(std::string_view source, ShaderType type);

    ShaderType Type() const;
    ::GLuint Native_Handle() const;

  private:
    AutoRelease<::GLuint> m_Handle;
    ShaderType m_Type;
};

}

template <>
struct std::formatter<game::ShaderType>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return std::cbegin(ctx);
    }

    auto format(const game::ShaderType& obj, std::format_context& ctx) const
    {
        switch (obj)
        {
            using enum game::ShaderType;
            case VERTEX: return std::format_to(ctx.out(), "VERTEX");
            case FRAGMENT: return std::format_to(ctx.out(), "FRAGMENT");
        }

        throw game::Exception("unknown shader type: {}", std::to_underlying(obj));
    }
};
