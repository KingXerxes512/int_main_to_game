#include <iostream>
#include <numbers>
#include <print>
#include <stacktrace>

#include "Camera.h"
#include "Error.h"
#include "Exception.h"
#include "KeyEvent.h"
#include "Log.h"
#include "Material.h"
#include "Matrix4.h"
#include "Mesh.h"
#include "Opengl.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Window.h"

namespace
{

static constexpr auto vertex_shader_src = R"(
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
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

}

int main()
{
    try
    {
        game::Window window(800u, 600u);

        const auto vertex_shader = game::Shader(vertex_shader_src, game::ShaderType::VERTEX);
        const auto fragment_shader = game::Shader(fragment_shader_src, game::ShaderType::FRAGMENT);
        auto material = game::Material(vertex_shader, fragment_shader);
        const auto mesh = game::Mesh();

        auto renderer = game::Renderer();

        static auto x = 2.0f;
        static auto y = 2.0f;
        static auto z = 0.0f;
        static auto t = 0.0f;

        x = std::sin(t) * 5.0f;
        y = std::cos(t) * 5.0f;
        z = std::cos(t) * 5.0f;

        t += 0.01f;

        auto camera = game::Camera(
            {.x = 3.0f, .y = 3.0f, .z = 10.0f},
            {.x = 0.0f, .y = 0.0f, .z = 0.0f},
            {.x = 0.0f, .y = 1.0f, .z = 0.0f},
            std::numbers::pi_v<float> / 4.0f,
            800.0f,
            600.0f,
            0.001f,
            100.0f);

        game::Entity e1(&mesh, &material, game::Vector3{0.0f, 0.0f, 0.0f});
        game::Entity e2(&mesh, &material, game::Vector3{2.5f, 2.0f, -1.5f});
        auto scene = game::Scene{.m_Entities = {&e1, &e2}};

        auto velocity = game::Vector3{.x = 0.0f, .y = 0.0f, .z = 0.0f};

        auto running = true;

        while (running)
        {
            auto event = window.PumpEvent();
            while (event && running)
            {
                std::visit(
                    [&](auto&& arg)
                    {
                        using T = std::decay_t<decltype(arg)>;

                        if constexpr (std::same_as<T, game::StopEvent>)
                        {
                            running = false;
                        }
                        else if constexpr (std::same_as<T, game::KeyEvent>)
                        {
                            if (arg.Key() == game::Key::ESC)
                            {
                                running = false;
                            }
                            else if (arg.Key() == game::Key::D)
                            {
                                velocity += arg.State() == game::KeyState::UP
                                                ? game::Vector3{.x = -0.5f, .y = 0.0f, .z = 0.0f}
                                                : game::Vector3{.x = 0.5f, .y = 0.0f, .z = 0.0f};
                            }
                            else if (arg.Key() == game::Key::A)
                            {
                                velocity += arg.State() == game::KeyState::UP
                                                ? game::Vector3{.x = 0.5f, .y = 0.0f, .z = 0.0f}
                                                : game::Vector3{.x = -0.5f, .y = 0.0f, .z = 0.0f};
                            }
                            else if (arg.Key() == game::Key::W)
                            {
                                velocity += arg.State() == game::KeyState::UP
                                                ? game::Vector3{.x = 0.0f, .y = 0.0f, .z = 0.5f}
                                                : game::Vector3{.x = 0.0f, .y = 0.0f, .z = -0.5f};
                            }
                            else if (arg.Key() == game::Key::S)
                            {
                                velocity += arg.State() == game::KeyState::UP
                                                ? game::Vector3{.x = 0.0f, .y = 0.0f, .z = -0.5f}
                                                : game::Vector3{.x = 0.0f, .y = 0.0f, .z = 0.5f};
                            }
                        }
                    },
                    *event);
                event = window.PumpEvent();
            }

            camera.Translate(game::Vector3::Normalize(velocity));

            renderer.Render(camera, scene);
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
