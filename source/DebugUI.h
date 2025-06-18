#pragma once

#include "Camera.h"
#include "MouseButtonEvent.h"
#include "Scene.h"

#include "imgui.h"

#include <windows.h>

namespace game
{

class DebugUI
{
  public:
    DebugUI(HWND window, Scene& scene, Camera& camera);
    ~DebugUI();

    void Render() const;
    void AddMouseEvent(const MouseButtonEvent event) const;

  private:
    Scene& m_Scene;
    Camera& m_Camera;
};

}
