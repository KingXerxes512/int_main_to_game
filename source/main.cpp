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

#include <chrono>
#include <iostream>
#include <numbers>
#include <print>
#include <ranges>
#include <stacktrace>
#include <unordered_map>

namespace
{

static constexpr auto vertex_shader_src = R"(
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertex_color;

uniform mat4 model;

layout(std140, binding = 0) uniform camera
{
    mat4 view;
    mat4 projection;
};

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

        auto entities = std::vector<game::Entity>{};

        for (auto i = -10; i < 10; ++i)
        {
            for (auto j = -10; j < 10; ++j)
            {
                entities.emplace_back(
                    &mesh, &material, game::Vector3{static_cast<float>(i) * 2.5f, 0.0f, static_cast<float>(j) * 2.5f});
            }
        }

        const auto scene = game::Scene{
            .m_Entities =
                entities | std::views::transform([](const auto& e) { return &e; }) | std::ranges::to<std::vector>()};

        auto camera = game::Camera(
            {3.0f, 3.0f, 10.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            std::numbers::pi_v<float> / 4.0f,
            800.0f,
            600.0f,
            0.001f,
            100.0f);

        /*game::Entity e1(&mesh, &material, game::Vector3{0.0f, 0.0f, 0.0f});
        game::Entity e2(&mesh, &material, game::Vector3{2.5f, 2.0f, -1.5f});
        auto scene = game::Scene{.m_Entities = {&e1, &e2}};*/

        std::unordered_map<game::Key, bool> key_state{
            {game::Key::W, false}, {game::Key::S, false}, {game::Key::S, false}, {game::Key::D, false}};

        auto running = true;

        const auto processEvents = [&](auto&& arg)
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

                key_state[arg.Key()] = arg.State() == game::KeyState::DOWN;
            }
            else if constexpr (std::same_as<T, game::MouseEvent>)
            {
                static constexpr float sensitivity = 0.001f;
                const float deltaX = arg.DeltaX() * sensitivity;
                const float deltaY = arg.DeltaY() * sensitivity;
                camera.AdjustYaw(deltaX);
                camera.AdjustPitch(-deltaY);
            }
        };

        using namespace std::literals::chrono_literals;
        constexpr auto targetTime = 8.333ms;
        auto startTime = std::chrono::system_clock::now();

        while (running)
        {
            startTime = std::chrono::system_clock::now();
            auto event = window.PumpEvent();
            while (event && running)
            {
                std::visit(processEvents, *event);
                event = window.PumpEvent();
            }

            game::Vector3 walkDirection{0.0f, 0.0f, 0.0f};
            walkDirection = game::Vector3::Normalize(walkDirection);

            if (key_state[game::Key::D])
            {
                walkDirection += camera.Right();
            }
            else if (key_state[game::Key::A])
            {
                walkDirection += -camera.Right();
            }
            else if (key_state[game::Key::W])
            {
                walkDirection += camera.Direction();
            }
            else if (key_state[game::Key::S])
            {
                walkDirection += -camera.Direction();
            }
            else if (key_state[game::Key::Q])
            {
                walkDirection += camera.Up();
            }
            else if (key_state[game::Key::E])
            {
                walkDirection += -camera.Up();
            }

            const auto speed = 0.01f;
            camera.Translate(game::Vector3::Normalize(walkDirection * speed));

            renderer.Render(camera, scene);
            window.Swap();

            auto end = std::chrono::system_clock::now();
            auto diff = targetTime - (end - startTime);
            std::this_thread::sleep_for(diff);
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
