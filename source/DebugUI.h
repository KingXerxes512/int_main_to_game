#pragma once

#include "MouseButtonEvent.h"
#include "Scene.h"

#include "imgui.h"

#include <windows.h>

namespace game
{

class DebugUI
{
  public:
    DebugUI(HWND window, Scene& scene);
    ~DebugUI();

    void Render() const;
    void AddMouseEvent(const MouseButtonEvent event) const;

  private:
      Scene& m_Scene;
};

}
