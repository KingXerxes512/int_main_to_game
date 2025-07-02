#include "Camera.h"
#include "DebugUI.h"
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
#include <random>
#include <ranges>
#include <stacktrace>
#include <unordered_map>

template <class T, class U>
void busySleep(std::chrono::duration<T, U> duration)
{
    auto end = std::chrono::system_clock::now() + duration;
    while (true)
    {
        auto now = std::chrono::system_clock::now();
        if (now > end)
            return;
    }
}

int main(int argc, char** argv)
{
    try
    {
        game::ensure(argc == 2, "Usage: ./game.exe <root_path>");

        game::Window window(1920, 1080);

        game::ResourceLoader resourceLoader{argv[1]};
        game::ModelLoader modelLoader{resourceLoader};

        const auto vertex_shader_src = resourceLoader.Load_String("simple_vert.glsl");
        const auto fragment_shader_src = resourceLoader.Load_String("simple_frag.glsl");

        game::Texture texture{resourceLoader.Load_Binary("container2.png"), 500, 500};
        game::Texture specMap{resourceLoader.Load_Binary("container2_specular.png"), 500, 500};
        game::Sampler sampler{};

        const game::Texture* textures[]{&texture, &specMap};
        const game::Sampler* samplers[]{&sampler, &sampler};
        const auto texSam = std::views::zip(textures, samplers) | std::ranges::to<std::vector>();

        //const auto sofaModel = modelLoader.Load("Low-Poly Plant_.obj", "Low-Poly_Plant_.001_Cube.000");
        const auto barrelModel = modelLoader.Load("Wooden-Barrel.obj", "Cylinder_Cylinder.002");
        const auto mesh = game::Mesh(barrelModel);
        const auto vertex_shader = game::Shader(vertex_shader_src, game::ShaderType::VERTEX);
        const auto fragment_shader = game::Shader(fragment_shader_src, game::ShaderType::FRAGMENT);
        auto material = game::Material(vertex_shader, fragment_shader);
        auto renderer = game::Renderer();

        auto entities = std::vector<game::Entity>{};

        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::uniform_real_distribution dist(-5.0f, 5.0f);

        entities.reserve(100);

        for (auto i = -10; i < 10; ++i)
        {
            for (auto j = -10; j < 10; ++j)
            {
                entities.emplace_back(
                    &mesh,
                    &material,
                    game::Vector3{static_cast<float>(i) * 3.5f, dist(gen), static_cast<float>(j) * 3.5f},
                    game::Vector3{0.1f},
                    texSam);
            }
        }

        auto scene = game::Scene{
            .entities =
                entities | std::views::transform([](const auto& e) { return &e; }) | std::ranges::to<std::vector>(),
            .ambient = {.r = 0.3f, .g = 0.3f, .b = 0.3f},                                                    //
            .directional = {.direction = {-1.0f, -1.0f, -1.0f}, .color = {.r = 0.5f, .g = 0.5f, .b = 0.5f}}, //
            .points =
                {
                    {.position = {5.0f, 5.0f, 0.0f},
                     .color = {.r = 0.0f, .g = 1.0f, .b = 0.0f},
                     .const_attenuation = 1.0f,
                     .linear_attenuation = 0.03f,
                     .quad_attenuation = 0.001f},
                    {.position = {-5.0f, 5.0f, 0.0f},
                     .color = {.r = 1.0f, .g = 0.0f, .b = 0.0f},
                     .const_attenuation = 1.0f,
                     .linear_attenuation = 0.03f,
                     .quad_attenuation = 0.001f} //
                } //
        };

        auto camera = game::Camera(
            {1.0f, 3.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            std::numbers::pi_v<float> / 4.0f,
            static_cast<float>(window.Width()),
            static_cast<float>(window.Height()),
            0.001f,
            300.0f);

        std::unordered_map<game::Key, bool> key_state{
            {game::Key::W, false}, {game::Key::S, false}, {game::Key::S, false}, {game::Key::D, false}};

        bool showDebug = true;
        game::DebugUI debugUI{window.Native_Handle(), scene, camera};

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

                if (arg.Key() == game::Key::F1 && arg.State() == game::KeyState::UP)
                {
                    showDebug = !showDebug;
                }
            }
            else if constexpr (std::same_as<T, game::MouseEvent>)
            {
                if (showDebug)
                    return;

                static constexpr float sensitivity = 0.001f;
                const float deltaX = arg.DeltaX() * sensitivity;
                const float deltaY = arg.DeltaY() * sensitivity;
                camera.AdjustYaw(deltaX);
                camera.AdjustPitch(-deltaY);
            }
            else if constexpr (std::same_as<T, game::MouseButtonEvent>)
            {
                debugUI.AddMouseEvent(arg);
            }
        };

        using namespace std::literals::chrono_literals;
        constexpr double targetFPS = 60.0;
        constexpr std::chrono::duration<double, std::milli> targetTime(1000.0 / targetFPS);
        game::log::debug("Target FPS: {}, frametime: ({:.3f}ms)", targetFPS, targetTime.count());

        while (running)
        {
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

            auto renderStart = std::chrono::system_clock::now();
            renderer.Render(camera, scene);

            if (showDebug)
                debugUI.Render();

            window.Swap();
            auto renderEnd = std::chrono::system_clock::now();

            auto diff = targetTime - (renderEnd - renderStart);
            busySleep(diff);
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
