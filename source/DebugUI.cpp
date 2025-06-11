#include "DebugUI.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"
#include "Log.h"

namespace game
{

DebugUI::DebugUI(HWND window, Scene& scene)
    : m_Scene(scene)
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
    ::ImGui_ImplOpenGL3_NewFrame();
    ::ImGui_ImplWin32_NewFrame();
    ::ImGui::NewFrame();

    static float color[3] = {};
    if (::ImGui::ColorPicker3("", color))
    {
        m_Scene.point.color.r = color[0];
        m_Scene.point.color.g = color[1];
        m_Scene.point.color.b = color[2];
    }

    ::ImGui::Render();
    ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}

void DebugUI::AddMouseEvent(const MouseButtonEvent event) const
{
    auto& io = ::ImGui::GetIO();
    io.AddMouseButtonEvent(0, event.State() == MouseButtonState::DOWN);
}

}
