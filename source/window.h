#pragma once

#include <cstdint>
#include <optional>

#include <Windows.h>

#include "AutoRelease.h"
#include "Event.h"

namespace game
{

class Window
{
  public:
    Window(std::uint32_t width, std::uint32_t height);
    ~Window() = default;

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;

    Window(Window&&) = default;
    Window& operator=(Window&&) = default;

    std::optional<Event> PumpEvent() const;
    void Swap() const;

  private:
    AutoRelease<::HWND, nullptr> m_Window;
    AutoRelease<::HDC> m_DC;
    WNDCLASSA m_WC;
};

}
