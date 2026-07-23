#include "glfwd_engine/application.h"

// TODO: SDL and glad should be removed later
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <fmt/base.h>
#include <glad/gl.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string_view>

#include "glfwd_core/resource_manager.h"
#include "glfwd_core/time.h"
#include "glfwd_core/window.h"
#include "glfwd_renderer/forward_renderer.h"

namespace glfwd {

// clang-format off
constexpr std::array<unsigned, 6> PlaneIndices = {
    0, 1, 2, // First triangle
    0, 2, 3, // Second triangle
};
// clang-format on

Application::~Application()
{
    if (m_Renderer)
    {
        delete m_ResourceManager;
        delete m_Renderer;
        delete m_Window;
        m_ResourceManager = nullptr;
        m_Renderer        = nullptr;
        m_Window          = nullptr;
    }
}

void Application::Initialize(const ApplicationCreateInfo& create_info)
{
    // Initialize resource manager
    m_ResourceManager = new ResourceManager(create_info.AssetDirectory);

    // Initialize window and backend contexts
    m_Renderer = new ForwardRenderer(m_ResourceManager, create_info.Renderer);
    m_Window   = new Window(create_info.Window);

    // Initialize forward renderer and RHI context
    m_Renderer->InitializeBackend(m_Window);
    m_Renderer->InitializeResources();

    OnInitialize();
}

void Application::Run()
{
    Timestep  timestep;
    SDL_Event event;
    while (m_Window->IsOpen())
    {
        while (SDL_PollEvent(&event))
        {
            m_Window->HandleSDLEvents(event);

            OnEvent(event);
        }

        timestep.CalculateDeltaTime();

        // TODO: Move to Forward renderer
        glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        OnUpdate(timestep);
        OnLateUpdate(timestep);
        SubmitToRenderQueue(m_Renderer->GetRenderQueue());

        m_Renderer->SwapBuffers();
    }
}

std::string_view Application::GetBasePath()
{
    const char* base_path = SDL_GetBasePath();
    GLFWD_ASSERT(base_path, "Base path is set to nullptr: {}", SDL_GetError());
    return std::string_view(base_path);
}

} // namespace glfwd
