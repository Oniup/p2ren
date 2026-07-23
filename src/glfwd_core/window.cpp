#include "glfwd_core/window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

#include "glfwd_core/utility/error.h"

namespace glfwd {

std::string_view Window::GetResolutionAsString(WindowResolution resolution)
{
    switch (resolution)
    {
    case WindowResolution::NHD:     return "NHD: (640, 360)";
    case WindowResolution::HD:      return "HD: (1280, 720)";
    case WindowResolution::FullHD:  return "FullHD: (1920, 1080)";
    case WindowResolution::QHD:     return "QHD: (2560, 1440)";
    case WindowResolution::QHDPlus: return "QHDPlus: (3200, 1800)";
    case WindowResolution::UHD:     return "UHD: (3840, 2160)";
    case WindowResolution::UHD5K:   return "UHD5k: (5120, 2880)";
    case WindowResolution::UHD8K:   return "UHD8k: (7680, 4320)";
    default:                        return "Custom";
    }
}

std::tuple<int32_t, int32_t> Window::GetResolutionSize(WindowResolution resolution)
{
    switch (resolution)
    {
    case WindowResolution::NHD:     return std::make_tuple(640, 360);
    case WindowResolution::HD:      return std::make_tuple(1280, 720);
    case WindowResolution::FullHD:  return std::make_tuple(1920, 1080);
    case WindowResolution::QHD:     return std::make_tuple(2560, 1440);
    case WindowResolution::QHDPlus: return std::make_tuple(3200, 1800);
    case WindowResolution::UHD:     return std::make_tuple(3840, 2160);
    case WindowResolution::UHD5K:   return std::make_tuple(5120, 2880);
    case WindowResolution::UHD8K:   return std::make_tuple(7680, 4320);
    default:                        return std::make_tuple(0, 0);
    }
}

WindowResolution Window::GetClosestResolutionSize(int32_t width)
{
    WindowResolution closest       = WindowResolution::Auto;
    int              closest_width = std::numeric_limits<int>::max();

    for (size_t i = 0; i < (size_t)WindowResolution::MaxCount; i++)
    {
        auto [resolution_width, _] = Window::GetResolutionSize((WindowResolution)i);

        int diff         = std::abs(width - resolution_width);
        int current_diff = std::abs(width - closest_width);
        if (diff < current_diff)
        {
            closest_width = resolution_width;
            closest       = (WindowResolution)i;
        }
    }
    return closest;
}

Window::Window(const WindowCreateInfo& info)
{
    SDL_DisplayID          display_id   = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(display_id);
    WindowFlags            flags        = info.Flags | SDL_WINDOW_OPENGL;

    int32_t          width      = info.Width;
    int32_t          height     = info.Height;
    WindowResolution resolution = info.Resolution;

    // Custom resolution
    if (info.Resolution == WindowResolution::Custom)
    {
        bool out_upper_bounds = info.Width > display_mode->w || info.Height > display_mode->h;
        bool out_lower_bounds = info.Width < 1 || info.Height < 0;
        if (out_upper_bounds || out_lower_bounds)
        {
            width  = display_mode->w / 2;
            height = display_mode->h / 2;

            GLFWD_WARN("Cannot set resolution size ({}, {}) as it exceeds monitor ({}, {}), using "
                       "fallback size ({}, {})",
                       info.Width,
                       info.Height,
                       display_mode->w,
                       display_mode->h,
                       width,
                       height);
        }
    }
    else // Not custom resolution, uses the enum to set the size
    {
        // Find closest resolution monitor
        if (resolution == WindowResolution::Auto)
            resolution = Window::GetClosestResolutionSize(display_mode->w - display_mode->w / 3);

        auto size = Window::GetResolutionSize(resolution);
        width     = std::get<0>(size);
        height    = std::get<1>(size);
    }

    m_Window = SDL_CreateWindow(info.Title.c_str(), width, height, flags);
    if (!m_Window)
        GLFWD_FATAL("Failed to create SDL window: {}", SDL_GetError());

    if (info.Resolution == WindowResolution::Custom)
        GLFWD_INFO("Create window with resolution: {}, {}", width, height);
    else
        GLFWD_INFO("Create window with resolution: {}, actual resolution: ({}, {})",
                   GetResolutionAsString(resolution),
                   width,
                   height);

    // Force the window to be focused when created
    if ((flags & SDL_WINDOW_ALWAYS_ON_TOP) == 0)
    {
        SDL_SetWindowAlwaysOnTop(m_Window, true);

        SDL_PumpEvents();
        SDL_DelayNS(1000);

        SDL_SetWindowAlwaysOnTop(m_Window, false);
    }
}

Window::~Window()
{
    Terminate();
}

Window::Window(Window&& other)
    : m_Window(other.m_Window),
      m_Open(other.m_Open)
{
    other.m_Window = nullptr;
    other.m_Open   = false;
}

Window& Window::operator=(Window&& other)
{
    Terminate();
    m_Window = other.m_Window;
    m_Open   = other.m_Open;

    other.m_Window = nullptr;
    other.m_Open   = false;
    return *this;
}

void Window::Terminate()
{
    if (m_Window)
        SDL_DestroyWindow(m_Window);

    m_Window = nullptr;
    m_Open   = false;
}

int32_t Window::GetWidth() const
{
    int32_t width;
    SDL_GetWindowSize(m_Window, &width, nullptr);
    return width;
}

int32_t Window::GetHeight() const
{
    int32_t height;
    SDL_GetWindowSize(m_Window, nullptr, &height);
    return height;
}

void Window::GetSize(int32_t* width, int32_t* height) const
{
    SDL_GetWindowSize(m_Window, width, height);
}

void Window::SetSize(int32_t width, int32_t height, bool center)
{
    SDL_DisplayID display_id = SDL_GetDisplayForWindow(m_Window);
    SetSize(display_id, width, height, center);
}

void Window::SetSize(WindowResolution resolution, bool center)
{
    SDL_DisplayID display_id = SDL_GetDisplayForWindow(m_Window);
    SetSize(display_id, resolution, center);
}

bool Window::IsOpen() const
{
    return m_Open;
}

void Window::HandleSDLEvents(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                                         event.window.windowID == SDL_GetWindowID(m_Window)))
    {
        m_Open = false;
    }
}

void Window::SetSize(uint32_t display_id, int32_t width, int32_t height, bool center)
{
    const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(display_id);

    if (width > display_mode->w || height > display_mode->h)
    {
        GLFWD_WARN("Cannot set resolution size ({}, {}) as it exceeds monitor ({}, {})",
                   width,
                   height,
                   display_mode->w,
                   display_mode->h);
        return;
    }

    SDL_SetWindowSize(m_Window, width, height);
    if (center)
    {
        SDL_SetWindowPosition(
            m_Window, display_mode->w / 2 - width / 2, display_mode->h / 2 - height / 2);
    }
}

void Window::SetSize(uint32_t display_id, WindowResolution& resolution, bool center)
{
    const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(display_id);

    if (resolution == WindowResolution::Auto)
        resolution = Window::GetClosestResolutionSize(display_mode->w - display_mode->w / 3);

    auto [width, height] = Window::GetResolutionSize(resolution);
    if (width > display_mode->w)
    {
        GLFWD_WARN("Cannot set resolution size ({}) as it exceeds monitor ({}, {})",
                   GetResolutionAsString(resolution),
                   display_mode->w,
                   display_mode->h);
        return;
    }

    SDL_SetWindowSize(m_Window, width, height);
    if (center)
    {
        SDL_SetWindowPosition(
            m_Window, display_mode->w / 2 - width / 2, display_mode->h / 2 - height / 2);
    }
}

} // namespace glfwd
