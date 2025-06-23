#include "DebugUI.h"

#include "Log.h"
#include "Matrix4.h"

#include "imgui.h"

#include "ImGuizmo.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_win32.h"

#include <format>
#include <ranges>

namespace game
{

DebugUI::DebugUI(HWND window, Scene& scene, Camera& camera)
    : m_Scene(scene)
    , m_Camera(camera)
{
    IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    ::ImGuiIO& io = ::ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    ::ImGui::StyleColorsDark();

    ::ImGui_ImplWin32_InitForOpenGL(window);
    ::ImGui_ImplOpenGL3_Init();
}

DebugUI::~DebugUI()
{
    ::ImGui_ImplOpenGL3_Shutdown();
    ::ImGui_ImplWin32_Shutdown();
    ::ImGui::DestroyContext();
}

void DebugUI::Render() const
{
    auto& io = ::ImGui::GetIO();

    ::ImGui_ImplOpenGL3_NewFrame();
    ::ImGui_ImplWin32_NewFrame();
    ::ImGui::NewFrame();

    static size_t selectedPointLight = 0;

    if (::ImGui::Button("Add Light"))
    {
        m_Scene.points.emplace_back(m_Scene.points.back());
        selectedPointLight = m_Scene.points.size() - 1;
    }

    ::ImGui::LabelText("", "FPS: %f", io.Framerate);

    for (const auto& [index, point] : m_Scene.points | std::views::enumerate)
    {
        float color[3] = {point.color.r, point.color.g, point.color.b};

        const auto name = std::format("color: {}", index);
        const auto constName = std::format("constant: {}", index);
        const auto linearName = std::format("linear: {}", index);
        const auto quadName = std::format("quadratic: {}", index);

        if (::ImGui::ColorPicker3(name.c_str(), color))
        {
            point.color.r = color[0];
            point.color.g = color[1];
            point.color.b = color[2];
            selectedPointLight = index;
        }

        ::ImGui::SliderFloat(constName.c_str(), &point.const_attenuation, 0.0f, 1.0f);
        ::ImGui::SliderFloat(linearName.c_str(), &point.linear_attenuation, 0.0f, 0.2f);
        ::ImGui::SliderFloat(quadName.c_str(), &point.quad_attenuation, 0.0f, 0.1f);
    }

    ::ImGuizmo::SetOrthographic(false);
    ::ImGuizmo::BeginFrame();
    ::ImGuizmo::Enable(true);
    ::ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    auto& point = m_Scene.points[selectedPointLight];
    Matrix4 translate = point.position;

    ::ImGuizmo::Manipulate(
        m_Camera.View().data(),
        m_Camera.Projection().data(),
        ::ImGuizmo::TRANSLATE,
        ::ImGuizmo::WORLD,
        const_cast<float*>(translate.Data().data()));

    point.position.x = translate.Data()[12];
    point.position.y = translate.Data()[13];
    point.position.z = translate.Data()[14];

    ::ImGui::Render();
    ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}

void DebugUI::AddMouseEvent(const MouseButtonEvent event) const
{
    auto& io = ::ImGui::GetIO();
    io.AddMouseButtonEvent(0, event.State() == MouseButtonState::DOWN);
}

}
