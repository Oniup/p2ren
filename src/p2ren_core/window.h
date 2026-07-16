#pragma once

#include <string>

struct SDL_Window;
union SDL_Event;

namespace p2ren {

enum class WindowResolution
{
    Custom,
    Auto,
    NHD,
    HD,
    FullHD,
    QHD,
    QHDPlus,
    UHD,
    UHD5K,
    UHD8K,
    MaxCount
};

// From SDL_video
enum WindowFlags_ : uint64_t
{
    WindowFlags_None              = 0,
    WindowFlags_Fullscreen        = 0x0000000000000001,
    WindowFlags_Borderless        = 0x0000000000000010,
    WindowFlags_Resizable         = 0x0000000000000020,
    WindowFlags_Minimized         = 0x0000000000000040,
    WindowFlags_Maximized         = 0x0000000000000080,
    WindowFlags_InputFocus        = 0x0000000000000200,
    WindowFlags_MouseFocus        = 0x0000000000000400,
    WindowFlags_External          = 0x0000000000000800,
    WindowFlags_HighPixelDensity  = 0x0000000000002000,
    WindowFlags_MouseCapture      = 0x0000000000004000,
    WindowFlags_MouseRelativeMode = 0x0000000000008000,
    WindowFlags_AlwaysOnTop       = 0x0000000000010000,
    WindowFlags_KeyboardGrabbed   = 0x0000000000100000,
    WindowFlags_Transparent       = 0x0000000040000000,
    WindowFlags_NotFocusable      = 0x0000000080000000,
};

using WindowFlags = uint64_t;

struct WindowCreateInfo
{
    std::string      Title      = "P2 Ren"; // Window display title
    int32_t          Width      = 800;      // Set Resolution to Custom to manually set size
    int32_t          Height     = 800;      // Set Resolution to Custom to manually set size
    WindowResolution Resolution = WindowResolution::Custom; // Set size based on this
    WindowFlags      Flags      = WindowFlags_Resizable;    // Window properties
};

class Window
{
public:
    static std::string_view             GetResolutionAsString(WindowResolution resolution);
    static std::tuple<int32_t, int32_t> GetResolutionSize(WindowResolution resolution);
    static WindowResolution             GetClosestResolutionSize(int32_t width);

    Window() = default;
    Window(const WindowCreateInfo& info);
    ~Window();

    Window(Window&& other);
    Window& operator=(Window&& other);

    Window(const Window& other)            = delete;
    Window& operator=(const Window& other) = delete;

    void Terminate();

    int32_t     GetWidth() const;
    int32_t     GetHeight() const;
    void        GetSize(int32_t* width, int32_t* height) const;
    SDL_Window* GetInternalContext() const { return m_Window; }

    void SetSize(int32_t width, int32_t height, bool center = true);
    void SetSize(WindowResolution resolution, bool center = true);

    bool IsOpen() const;
    void HandleSDLEvents(const SDL_Event& event);

private:
    void SetSize(uint32_t display_id, int32_t width, int32_t height, bool center);
    void SetSize(uint32_t display_id, WindowResolution& resolution, bool center);

    SDL_Window* m_Window = nullptr;
    bool        m_Open   = true;
};

} // namespace p2ren
