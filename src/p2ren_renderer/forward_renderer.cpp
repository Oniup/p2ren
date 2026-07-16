#include "p2ren_renderer/forward_renderer.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "p2ren_core/resource_manager.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/create_info.h"
#include "p2ren_renderer/rhi/context.h"
#include "p2ren_renderer/rhi/shader.h"

namespace p2ren {

ForwardRenderer::ForwardRenderer(ResourceManager* resources_manager, const RendererCreateInfo& info)
    : m_ResourceManager(resources_manager),
      m_Context(new RHIContext(info.EnableOpenGLDebugCallback)) // SDL and OpenGL Attributes
{
}

ForwardRenderer::~ForwardRenderer()
{
    if (m_Context)
    {
        delete m_Context;
        m_Context = nullptr;
    }
}

void ForwardRenderer::InitializeBackend(Window* window)
{
    m_Window = window;
    m_Context->InitializeBackend(window);
}

void ForwardRenderer::InitializeResources()
{
    // Initialize pools
    m_ResourceManager->InitializePool<Shader>(ShaderPoolInitialCapacity);

    // Initialize default resources
    m_ResourceManager->PushResource<Shader>(
        "Flat Color",
        Shader(m_ResourceManager->GetAssetPath("shaders/base.vert"),
               m_ResourceManager->GetAssetPath("shaders/flat_color.frag")));
}

void ForwardRenderer::SwapBuffers()
{
    SDL_GL_SwapWindow(m_Window->GetInternalContext());
}

} // namespace p2ren
