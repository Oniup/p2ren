#pragma once

typedef struct SDL_GLContextState* SDL_GLContext;

namespace p2ren {

struct RendererDescriptor;
class Window;

class RHI_Context
{
public:
    ~RHI_Context();

    void Initialize(const RendererDescriptor& descriptor);
    void InitializeBackend(const Window& window, const RendererDescriptor& descriptor);
    void Terminate();

    SDL_GLContext       GetInternalContext() { return m_InternalContext; }
    const SDL_GLContext GetInternalContext() const { return m_InternalContext; }

    void SwapBuffers(const Window& window);

private:
    SDL_GLContext m_InternalContext = nullptr;
};

} // namespace p2ren
