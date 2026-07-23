#include "glfwd_renderer/forward_renderer.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "glfwd_core/resource_manager.h"
#include "glfwd_core/window.h"
#include "glfwd_renderer/create_info.h"
#include "glfwd_renderer/rhi/context.h"
#include "glfwd_renderer/rhi/shader.h"
#include "glfwd_renderer/rhi/texture.h"

namespace glfwd {

ForwardRenderer::ForwardRenderer(ResourceManager* resources_manager, const RendererCreateInfo& info)
    : m_ResourceManager(resources_manager),
      m_Context(new OpenGLContext(info.EnableOpenGLDebugCallback)) // SDL and OpenGL Attributes
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
    m_ResourceManager->InitializePool<Shader>();
    m_ResourceManager->InitializePool<Texture>();

    // Initialize default Shaders
    // ---------------------------------------------------------------------------------------------
    m_ResourceManager->PushResource<Shader>(
        "Flat Color",
        Shader(m_ResourceManager->GetAssetPath("shaders/base.vert"),
               m_ResourceManager->GetAssetPath("shaders/flat_color.frag")));

    m_ResourceManager->PushResource<Shader>(
        "Basic Blinn-Phong",
        Shader(m_ResourceManager->GetAssetPath("shaders/base.vert"),
               m_ResourceManager->GetAssetPath("shaders/base-blinn-phong.frag")));

    // Initialize default Textures
    // ---------------------------------------------------------------------------------------------
    ResourceHandle default_material_white = m_ResourceManager->PushResource<Texture>(
        "Default Texture",
        Texture(m_ResourceManager->GetAssetPath("textures/default.png"),
                TextureCreateInfo{
                    // can afford terrible mipmaps as the texture is only white
                    .Mipmap = MipmapMode::Nearest,
                }));

    intern::InitializeDefaultImageAssignment(default_material_white);
}

void ForwardRenderer::SwapBuffers()
{
    SDL_GL_SwapWindow(m_Window->GetInternalContext());
}

} // namespace glfwd
