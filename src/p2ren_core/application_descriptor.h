#pragma once

#include "p2ren_core/utility/error.h"
#include "p2ren_core/window.h"

namespace p2ren {

struct RendererDescriptor
{
    bool EnableVSync               = true;
    bool EnableOpenGLDebugCallback = true;
};

struct WindowDescriptor
{
    std::string      Title      = "P2 Ren"; // Window display title
    int32_t          Width      = 800;      // Set Resolution to Custom to manually set size
    int32_t          Height     = 800;      // Set Resolution to Custom to manually set size
    WindowResolution Resolution = WindowResolution::Auto; // Set size based on this
    WindowFlags      Flags      = WindowFlags_Resizable;  // Window properties
};

struct ApplicationDescriptor
{
    RendererDescriptor Renderer;
    WindowDescriptor   Window;
    std::string        AssetDirectory; // Leave empty to find assets/ in parents directories
    ErrorSeverity ErrorLogMinSeverityFilter = ErrorSeverity::Low; // Leave to show all log messages
};

} // namespace p2ren
