#include <print>
#include <stdexcept>

#include "error.h"
#include "window.h"

namespace
{

bool g_Running = true;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
        case WM_CLOSE: g_Running = false; break;
        case WM_KEYDOWN: std::println("key down"); break;
    }

    return ::DefWindowProcA(hWnd, Msg, wParam, lParam);
}

}

namespace game
{

Window::Window(std::uint32_t width, std::uint32_t height)
    : window_({})
    , wc_({})
{
    wc_ = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = WindowProc,
        .hInstance = ::GetModuleHandleA(nullptr),
        .lpszClassName = "window class"};

    ensure(::RegisterClassA(&wc_) != 0, "Failed to register class!");

    RECT rect{.left = {}, .top = {}, .right = static_cast<long>(width), .bottom = static_cast<long>(height)};

    ensure(::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false) != 0, "Could not resize window!");

    window_ = {
        ::CreateWindowExA(
            0,
            wc_.lpszClassName,
            "game window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            wc_.hInstance,
            nullptr),
        ::DestroyWindow};

    ::ShowWindow(window_, SW_SHOW);
    ::UpdateWindow(window_);
}

bool Window::Running() const
{
    ::MSG message{};

    while (::PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE) != 0)
    {
        ::TranslateMessage(&message);
        ::DispatchMessageA(&message);
    }

    return g_Running;
}

}
