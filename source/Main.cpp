#include "Camera.h"
#include "Entity.h"
#include "Error.h"
#include "Exception.h"
#include "KeyEvent.h"
#include "Log.h"
#include "Material.h"
#include "Matrix4.h"
#include "Mesh.h"
#include "Opengl.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Sampler.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

#include <chrono>
#include <iostream>
#include <numbers>
#include <print>
#include <ranges>
#include <stacktrace>
#include <unordered_map>

int main(int argc, char** argv)
{
    try
    {
        game::ensure(argc == 2, "Usage: ./game.exe <root_path>");

        game::Window window(800u, 600u);

        game::ResourceLoader loader{argv[1]};
        const auto vertex_shader_src = loader.Load_String("simple_vert.glsl");
        const auto fragment_shader_src = loader.Load_String("simple_frag.glsl");

        game::Texture texture{loader.Load_Binary("container2.png"), 500, 500};
        game::Texture specMap{loader.Load_Binary("container2_specular.png"), 500, 500};
        game::Sampler sampler{};

        const game::Texture* textures[]{&texture, &specMap};
        const game::Sampler* samplers[]{&sampler, &sampler};
        const auto texSam = std::views::zip(textures, samplers) | std::ranges::to<std::vector>();

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
                    &mesh,
                    &material,
                    game::Vector3{static_cast<float>(i) * 3.5f, 0.0f, static_cast<float>(j) * 3.5f},
                    texSam);
            }
        }

        auto scene = game::Scene{
            .entities =
                entities | std::views::transform([](const auto& e) { return &e; }) | std::ranges::to<std::vector>(),
            .ambient = {.r = 0.3f, .g = 0.3f, .b = 0.3f},                                                    //
            .directional = {.direction = {-1.0f, -1.0f, -1.0f}, .color = {.r = 0.5f, .g = 0.5f, .b = 0.5f}}, //
            .point = {.position = {5.0f, 5.0f, 0.0f}, .color = {.r = 0.5f, .g = 0.5f, .b = 0.5f}}            //
        };

        auto camera = game::Camera(
            {1.0f, 3.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            std::numbers::pi_v<float> / 4.0f,
            800.0f,
            600.0f,
            0.001f,
            100.0f);

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

            static auto t = 0.0f;
            t += 0.01f;

            scene.point.position.x = std::sin(t) * 10.0f;
            scene.point.position.z = std::cos(t) * 10.0f;

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
