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

    HWND Native_Handle() const
    {
        return m_Window;
    }

    uint32_t Width() const
    {
        return m_Width;
    }
    
    uint32_t Height() const
    {
        return m_Height;
    }

  private:
    AutoRelease<::HWND, nullptr> m_Window;
    AutoRelease<::HDC> m_DC;
    WNDCLASSA m_WC;
    uint32_t m_Width;
    uint32_t m_Height;
};

}
