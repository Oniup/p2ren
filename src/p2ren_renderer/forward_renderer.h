#pragma once

typedef struct SDL_GLContextState* SDL_GLContext;

namespace p2ren {

class Window;
class ResourceManager;

struct RendererCreateInfo;
class RHIContext;

class ForwardRenderer
{
public:
    static constexpr size_t ShaderPoolInitialCapacity = 20;

    ForwardRenderer(ResourceManager* resources_manager, const RendererCreateInfo& info);
    ~ForwardRenderer();

    void InitializeBackend(Window* window);
    void InitializeResources();

    void SwapBuffers();

private:
    Window*          m_Window          = nullptr;
    ResourceManager* m_ResourceManager = nullptr;
    RHIContext*      m_Context         = nullptr;
};

} // namespace p2ren
