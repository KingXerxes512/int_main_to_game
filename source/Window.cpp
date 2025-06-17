#include <print>
#include <queue>

#include "Error.h"
#include "Exception.h"
#include "Key.h"
#include "KeyEvent.h"
#include "Log.h"
#include "MouseButtonEvent.h"
#include "Opengl.h"
#include "StopEvent.h"
#include "Window.h"

#include <Windowsx.h>
#include <hidusage.h>

namespace
{

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB{};
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB{};

std::queue<game::Event> g_EventQueue;

void APIENTRY OpenGLDebugCallback(
    ::GLenum source,
    ::GLenum type,
    ::GLenum id,
    ::GLenum severity,
    ::GLsizei length,
    const ::GLchar* message,
    const void*)
{
    std::string sev = "?";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_LOW: sev = "GL_DEBUG_SEVERITY_LOW"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: sev = "GL_DEBUG_SEVERITY_MEDIUM"; break;
        case GL_DEBUG_SEVERITY_HIGH: sev = "GL_DEBUG_SEVERITY_HIGH"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "GL_DEBUG_SEVERITY_NOTIFICATION"; return;
    }

    std::string ty = "?";
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
        {
            ty = "GL_DEBUG_TYPE_ERROR";
            throw game::Exception(std::format("{} {} {} {} {} {}", source, ty, id, sev, length, message));
        }
        case GL_DEBUG_TYPE_OTHER:
        {
            ty = "GL_DEBUG_TYPE_OTHER";
            game::log::debug("{} {} {} {} {} {}", source, ty, id, sev, length, message);
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        {
            ty = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            game::log::debug("{} {} {} {} {} {}", source, ty, id, sev, length, message);
            break;
        }
        case GL_DEBUG_TYPE_PERFORMANCE:
        {
            ty = "GL_DEBUG_TYPE_PERFORMANCE";
            game::log::warn("{} {} {} {} {} {}", source, ty, id, sev, length, message);
            break;
        }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        {
            ty = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            game::log::warn("{} {} {} {} {} {}", source, ty, id, sev, length, message);
            break;
        }
        default:
        {
            game::log::debug("{} {} {} {} {} {}", source, type, id, sev, length, message);
            break;
        }
    }
}

bool g_Running = true;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
        case WM_CLOSE: g_EventQueue.emplace(game::StopEvent()); break;
        case WM_KEYUP:
        {
            g_EventQueue.emplace(game::KeyEvent(static_cast<game::Key>(wParam), game::KeyState::UP));
            break;
        }
        case WM_KEYDOWN:
        {
            g_EventQueue.emplace(game::KeyEvent(static_cast<game::Key>(wParam), game::KeyState::DOWN));
            break;
        }
        case WM_INPUT:
        {
            auto raw = ::RAWINPUT{};
            auto dwSize = UINT{sizeof(::RAWINPUT)};
            game::ensure(
                ::GetRawInputData(
                    reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &dwSize, sizeof(::RAWINPUTHEADER)) !=
                    static_cast<::UINT>(-1),
                "Failed to get raw input.");

            if (raw.header.dwType == RIM_TYPEMOUSE)
            {
                const auto x = raw.data.mouse.lLastX;
                const auto y = raw.data.mouse.lLastY;

                g_EventQueue.emplace(game::MouseEvent(static_cast<float>(x), static_cast<float>(y)));
            }

            break;
        }
        case WM_LBUTTONUP:
        {
            game::log::debug("Left button up");
            g_EventQueue.emplace(game::MouseButtonEvent(
                static_cast<float>(GET_X_LPARAM(lParam)),
                static_cast<float>(GET_Y_LPARAM(lParam)),
                game::MouseButtonState::UP));
            break;
        }
        case WM_LBUTTONDOWN:
        {
            game::log::debug("Left button down");
            g_EventQueue.emplace(game::MouseButtonEvent(
                static_cast<float>(GET_X_LPARAM(lParam)),
                static_cast<float>(GET_Y_LPARAM(lParam)),
                game::MouseButtonState::DOWN));
            break;
        }
    }

    return ::DefWindowProcA(hWnd, Msg, wParam, lParam);
}

template <class T>
void ResolveGLFunction(T& function, const std::string& name)
{
    const auto address = ::wglGetProcAddress(name.c_str());
    game::ensure(address != nullptr, "Failed to resolve {}", name);

    function = reinterpret_cast<T>(address);
}

void ResolveGLFunctions(HINSTANCE instance)
{
    ::WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = ::DefWindowProc,
        .hInstance = instance,
        .lpszClassName = "dummy window",
    };
    game::ensure(::RegisterClassA(&wc) != 0, "Failed to register dummy window!");

    auto dummy_window = game::AutoRelease<::HWND>(
        ::CreateWindowExA(
            0,
            wc.lpszClassName,
            wc.lpszClassName,
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            wc.hInstance,
            0),
        ::DestroyWindow);
    game::ensure(dummy_window, "Failed to create dummy window!");

    auto dc =
        game::AutoRelease<::HDC>(::GetDC(dummy_window), [&dummy_window](auto dc) { ::ReleaseDC(dummy_window, dc); });
    game::ensure(dc, "Failed to get dummy DC!");

    auto pfd = ::PIXELFORMATDESCRIPTOR{
        .nSize = sizeof(::PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .cStencilBits = 8,
        .iLayerType = 24,
    };

    auto pixel_format = ::ChoosePixelFormat(dc, &pfd);
    game::ensure(pixel_format != 0, "Failed to choose a pixel format!");
    game::ensure(::SetPixelFormat(dc, pixel_format, &pfd) != false, "Failed to set pixel format!");

    const auto context = game::AutoRelease<::HGLRC>(::wglCreateContext(dc), ::wglDeleteContext);
    game::ensure(context, "Failed to create wgl context!");

    game::ensure(::wglMakeCurrent(dc, context) == TRUE, "Failed to make current context!");

    ResolveGLFunction(wglCreateContextAttribsARB, "wglCreateContextAttribsARB");
    ResolveGLFunction(wglChoosePixelFormatARB, "wglChoosePixelFormatARB");

    game::ensure(::wglMakeCurrent(dc, 0) == TRUE, "Failed to unbind context!");
}

void SetupOpenGLDebug()
{
    ::glEnable(GL_DEBUG_OUTPUT);
    ::glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    ::glDebugMessageCallback(OpenGLDebugCallback, nullptr);
}

}

void InitOpenGL(HDC dc)
{
    int pixelFormatAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,
        GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,
        GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,
        GL_TRUE,
        WGL_ACCELERATION_ARB,
        WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,
        WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,
        32,
        WGL_DEPTH_BITS_ARB,
        24,
        WGL_STENCIL_BITS_ARB,
        8,
        0,
    };

    int pixelFormat = 0;
    UINT numFormats{};
    ::wglChoosePixelFormatARB(dc, pixelFormatAttribs, 0, 1, &pixelFormat, &numFormats);
    game::ensure(numFormats != 0u, "Failed to choose a pixel format");

    ::PIXELFORMATDESCRIPTOR pfd = {};
    game::ensure(::DescribePixelFormat(dc, pixelFormat, sizeof(pfd), &pfd) != 0, "Failed to describe pixel format!");
    game::ensure(::SetPixelFormat(dc, pixelFormat, &pfd) == TRUE, "Failed to set pixel format!");

    int glAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        4,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        6,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    auto context = ::wglCreateContextAttribsARB(dc, 0, glAttribs);
    game::ensure(context != nullptr, "Failed to create opengl context!");
    game::ensure(::wglMakeCurrent(dc, context) == TRUE, "Failed to make current context!");
}

void ResolveGlobalGLFunctions()
{
#define RESOLVE(TYPE, NAME) ResolveGLFunction(NAME, #NAME);
    FOR_OPENGL_FUNCTIONS(RESOLVE)
}

namespace game
{

Window::Window(std::uint32_t width, std::uint32_t height)
    : m_Window({})
    , m_DC({})
    , m_WC({})
{
    m_WC = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = WindowProc,
        .hInstance = ::GetModuleHandleA(nullptr),
        .lpszClassName = "window class",
    };

    ensure(::RegisterClassA(&m_WC) != 0, "Failed to register class!");

    RECT rect{
        .left = {},
        .top = {},
        .right = static_cast<long>(width),
        .bottom = static_cast<long>(height),
    };

    ensure(::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false) != 0, "Failed to resize window!");

    m_Window = {
        ::CreateWindowExA(
            0,
            m_WC.lpszClassName,
            "game window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            m_WC.hInstance,
            nullptr),
        ::DestroyWindow};

    m_DC = AutoRelease<HDC>(::GetDC(m_Window), [this](auto dc) { ::ReleaseDC(m_Window, dc); });

    ::ShowWindow(m_Window, SW_SHOW);
    ::UpdateWindow(m_Window);

    const ::RAWINPUTDEVICE rid = ::RAWINPUTDEVICE{
        .usUsagePage = HID_USAGE_PAGE_GENERIC,
        .usUsage = HID_USAGE_GENERIC_MOUSE,
        .dwFlags = RIDEV_INPUTSINK,
        .hwndTarget = m_Window};

    ensure(::RegisterRawInputDevices(&rid, 1, sizeof(rid)) == TRUE, "failed to register input device");

    ResolveGLFunctions(m_WC.hInstance);
    InitOpenGL(m_DC);
    ResolveGlobalGLFunctions();
    SetupOpenGLDebug();

    ::wglSwapIntervalEXT(1);
    ::glEnable(GL_DEPTH_TEST);
}

std::optional<Event> Window::PumpEvent() const
{
    ::MSG message{};

    while (::PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE) != 0)
    {
        ::TranslateMessage(&message);
        ::DispatchMessageA(&message);
    }

    if (!std::ranges::empty(g_EventQueue))
    {
        const auto event = g_EventQueue.front();
        g_EventQueue.pop();
        return event;
    }

    return {};
}

void Window::Swap() const
{
    ::SwapBuffers(m_DC);
}

}
