#pragma once

#include <cstdint>

#include "p2ren_renderer/draw_modes.h"

typedef struct SDL_GLContextState* SDL_GLContext;

namespace p2ren {

class Window;

class RHIContext
{
public:
    static int32_t ConvertPolygonModeToRHI(PolygonMode mode);
    static int32_t ConvertPrimitiveModeToRHI(PrimitiveMode mode);

    RHIContext(bool enable_hardware_debug_callback);
    ~RHIContext();

    void InitializeBackend(Window* window);
    void Terminate();

    SDL_GLContext       GetInternalContext() { return m_InternalContext; }
    const SDL_GLContext GetInternalContext() const { return m_InternalContext; }

private:
    SDL_GLContext m_InternalContext = nullptr;
};

} // namespace p2ren
