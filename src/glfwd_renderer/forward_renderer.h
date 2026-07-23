#pragma once

#include "glfwd_renderer/render_queue.h"
typedef struct SDL_GLContextState* SDL_GLContext;

namespace glfwd {

class Window;
class ResourceManager;

struct RendererCreateInfo;
class OpenGLContext;

class ForwardRenderer
{
public:
    ForwardRenderer(ResourceManager* resources_manager, const RendererCreateInfo& info);
    ~ForwardRenderer();

    void InitializeBackend(Window* window);
    void InitializeResources();

    RenderQueue*       GetRenderQueue() { return &m_RenderQueue; }
    const RenderQueue* GetRenderQueue() const { return &m_RenderQueue; }

    void SwapBuffers();

private:
    RenderQueue      m_RenderQueue;
    Window*          m_Window          = nullptr;
    ResourceManager* m_ResourceManager = nullptr;
    OpenGLContext*   m_Context         = nullptr;
};

} // namespace glfwd
