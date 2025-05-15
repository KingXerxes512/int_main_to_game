#pragma once

#include <cstdint>

#include "auto_release.h"
#include <Windows.h>

namespace game
{

class Window
{
  public:
    Window(std::uint32_t width, std::uint32_t height);
    ~Window() = default;

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;

    // Window(const Window&&) = default;
    // Window operator=(Window&&) = default;

    bool Running() const;

  private:
    AutoRelease<HWND, nullptr> window_;
    WNDCLASSA wc_;
};

}
