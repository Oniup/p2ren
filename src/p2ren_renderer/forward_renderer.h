#pragma once

typedef struct SDL_GLContextState* SDL_GLContext;

namespace p2ren {

struct RendererDescriptor;
class RHIContext;

class ForwardRenderer
{
public:
    static constexpr size_t ShaderPoolInitialCapacity = 20;

    ForwardRenderer(const RendererDescriptor& descriptor);
    ~ForwardRenderer();

    void InitializeBackend();
    void InitializeResourcePools();

    void SwapBuffers();

private:
    RHIContext* m_Context = nullptr;
};

} // namespace p2ren
