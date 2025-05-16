#include <iostream>
#include <print>
#include <stacktrace>

#include "error.h"
#include "exception.h"
#include "opengl.h"
#include "window.h"

namespace
{

static constexpr auto vertex_shader_src = R"(
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vertex_color;

void main()
{
    gl_Position = vec4(position, 1.0);
    vertex_color = color;
}

)";

static constexpr auto fragment_shader_src = R"(
#version 460 core

in vec3 vertex_color;
out vec4 frag_color;

void main()
{
    frag_color = vec4(vertex_color, 1.0);
}

)";

game::AutoRelease<GLuint> compile_shader(std::string_view source, ::GLenum shader_type)
{
    auto shader = game::AutoRelease<::GLuint>(::glCreateShader(shader_type), ::glDeleteShader);

    const ::GLchar* strings = {source.data()};
    const ::GLint lengths[] = {static_cast<::GLint>(source.length())};

    ::glShaderSource(shader, 1, &strings, lengths);
    ::glCompileShader(shader);

    ::GLint result;
    ::glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    game::ensure(result, "Failed to compile shader {}", shader_type);

    return shader;
}

}

int main()
{
    // clang-format off
    static constexpr float vertex_data[] = {
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 1.0f
    };
    // clang-format on

    try
    {
        game::Window window(800u, 600u);

        auto vertex_shader = compile_shader(vertex_shader_src, GL_VERTEX_SHADER);
        auto fragment_shader = compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER);

        auto program = game::AutoRelease<::GLuint>(::glCreateProgram(), ::glDeleteProgram);
        game::ensure(program, "Failed to create opengl program!");

        ::glAttachShader(program, vertex_shader);
        ::glAttachShader(program, fragment_shader);
        ::glLinkProgram(program);

        ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        ::GLuint vao{};
        ::glGenVertexArrays(1, &vao);

        ::GLuint vbo{};
        ::glGenBuffers(1, &vbo);

        ::glBindVertexArray(vao);

        ::glBindBuffer(GL_ARRAY_BUFFER, vbo);
        ::glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

        ::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
        ::glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        ::glEnableVertexAttribArray(0);
        ::glEnableVertexAttribArray(1);

        ::glBindVertexArray(0);

        while (window.Running())
        {
            ::glClear(GL_COLOR_BUFFER_BIT);

            ::glUseProgram(program);
            ::glBindVertexArray(vao);
            ::glDrawArrays(GL_TRIANGLES, 0, 3);

            window.Swap();
        }
    }
    catch (const game::Exception& err)
    {
        std::println(std::cerr, "{}", err);
    }
    catch (...)
    {
        std::println(std::cerr, "unknown exception");
    }

    return 0;
}
