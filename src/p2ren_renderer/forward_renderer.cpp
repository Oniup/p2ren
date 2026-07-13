#include "p2ren_renderer/forward_renderer.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "p2ren_core/application.h"
#include "p2ren_core/application_descriptor.h"
#include "p2ren_core/resource_manager.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/rhi/context.h"
#include "p2ren_renderer/rhi/shader.h"

namespace p2ren {

ForwardRenderer::ForwardRenderer(const RendererDescriptor& descriptor)
{
    m_Context = new RHIContext(descriptor.EnableOpenGLDebugCallback); // SDL and OpenGL Attributes
}

ForwardRenderer::~ForwardRenderer()
{
    if (m_Context)
    {
        delete m_Context;
        m_Context = nullptr;
    }
}

void ForwardRenderer::InitializeBackend()
{
    m_Context->InitializeBackend();
}

void ForwardRenderer::InitializeResourcePools()
{
    ResourceManager* resources = Application::GetResourceManager();

    resources->InitializePool<Shader>(ShaderPoolInitialCapacity);
}

void ForwardRenderer::SwapBuffers()
{
    SDL_Window* window = Application::GetWindow()->GetInternalContext();
    SDL_GL_SwapWindow(window);
}

} // namespace p2ren
