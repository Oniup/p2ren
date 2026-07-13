#pragma once

typedef struct SDL_GLContextState* SDL_GLContext;

namespace p2ren {

class RHIContext
{
public:
    RHIContext(bool enable_hardware_debug_callback);
    ~RHIContext();

    void InitializeBackend();
    void Terminate();

    SDL_GLContext       GetInternalContext() { return m_InternalContext; }
    const SDL_GLContext GetInternalContext() const { return m_InternalContext; }

private:
    SDL_GLContext m_InternalContext = nullptr;
};

} // namespace p2ren
