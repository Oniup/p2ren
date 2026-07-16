#pragma once

#include "p2ren_core/utility/error.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/create_info.h "

namespace p2ren {

struct ApplicationCreateInfo
{
    RendererCreateInfo Renderer;
    WindowCreateInfo   Window;
    std::string        AssetDirectory; // Leave empty to find assets/ in parents directories
    ErrorSeverity ErrorLogMinSeverityFilter = ErrorSeverity::Low; // Leave to show all log messages
};

} // namespace p2ren
