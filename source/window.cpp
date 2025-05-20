#include <print>
#include <queue>

#include "Key.h"
#include "KeyEvent.h"
#include "Log.h"
#include "StopEvent.h"
#include "error.h"
#include "opengl.h"
#include "window.h"

namespace
{

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB{};
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB{};

std::queue<game::Event> g_EventQueue;

void APIENTRY opengl_debug_callback(
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
        case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "GL_DEBUG_SEVERITY_NOTIFICATION"; break;
    }

    std::println("{} {} {} {} {} {}", source, type, id, sev, length, message);
}

bool g_Running = true;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
        case WM_CLOSE: g_EventQueue.emplace(game::StopEvent()); break;
        case WM_KEYDOWN:
        {
            g_EventQueue.emplace(game::KeyEvent(static_cast<game::Key>(wParam)));
            break;
        }
    }

    return ::DefWindowProcA(hWnd, Msg, wParam, lParam);
}

template <class T>
void resolve_gl_function(T& function, const std::string& name)
{
    const auto address = ::wglGetProcAddress(name.c_str());
    game::ensure(address != nullptr, "Failed to resolve {}", name);

    function = reinterpret_cast<T>(address);
}

void resolve_wgl_functions(HINSTANCE instance)
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

    resolve_gl_function(wglCreateContextAttribsARB, "wglCreateContextAttribsARB");
    resolve_gl_function(wglChoosePixelFormatARB, "wglChoosePixelFormatARB");

    game::ensure(::wglMakeCurrent(dc, 0) == TRUE, "Failed to unbind context!");
}

void setup_opengl_debug()
{
    ::glEnable(GL_DEBUG_OUTPUT);
    ::glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    ::glDebugMessageCallback(opengl_debug_callback, nullptr);
}

}

void init_opengl(HDC dc)
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

void resolve_global_gl_functions()
{
#define RESOLVE(TYPE, NAME) resolve_gl_function(NAME, #NAME);
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

    m_DC = game::AutoRelease<HDC>(::GetDC(m_Window), [this](auto dc) { ::ReleaseDC(m_Window, dc); });

    ::ShowWindow(m_Window, SW_SHOW);
    ::UpdateWindow(m_Window);

    resolve_wgl_functions(m_WC.hInstance);
    init_opengl(m_DC);
    resolve_global_gl_functions();
    setup_opengl_debug();

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
