#include "p2ren_core/application.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <fmt/base.h>

#include <filesystem>
#include <string_view>

#include "p2ren_core/resource_manager.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/forward_renderer.h"
#include "p2ren_renderer/rhi/shader.h"

namespace p2ren {

Application* Application::s_Application = nullptr;

Application::Application()
{
    P2REN_ASSERT(s_Application == nullptr,
                 "Cannot have two application instances in memory running at the same time");

    s_Application = this;
}

Application::~Application()
{
    if (m_Renderer)
    {
        delete m_Renderer;
        delete m_Window;
        delete m_ResourceManager;
        m_Renderer        = nullptr;
        m_Window          = nullptr;
        m_ResourceManager = nullptr;
    }
}

void Application::Initialize(const ApplicationDescriptor& descriptor)
{
    // Initialize resource manager
    m_ResourceManager = new ResourceManager(
        descriptor.AssetDirectory.empty() ? FindAssetDirectory() : descriptor.AssetDirectory);

    // Initialize window
    m_Renderer = new ForwardRenderer(descriptor.Renderer);
    if (descriptor.Window.Resolution == WindowResolution::Custom)
    {
        m_Window = new Window(descriptor.Window.Title,
                              descriptor.Window.Width,
                              descriptor.Window.Height,
                              descriptor.Window.Flags);
    }
    else
    {
        m_Window = new Window(
            descriptor.Window.Title, descriptor.Window.Resolution, descriptor.Window.Flags);
    }

    // Initialize forward renderer
    m_Renderer->InitializeBackend();
    m_Renderer->InitializeResourcePools();

    // Test loading flat color shader
    m_ResourceManager->PushResource<Shader>(
        "Flat Color shader",
        Shader(m_ResourceManager->GetAssetPath("shaders/base.vert"),
               m_ResourceManager->GetAssetPath("shaders/flat_color.frag")));
}

std::string Application::FindAssetDirectory()
{
    namespace fs = std::filesystem;

    std::string_view base_path(GetBasePath());
    fs::path         working_directory(base_path);

    // Iterate over parent paths to check if it contains an "assets" directory
    while (true)
    {
        // Check if target asset path exists
        fs::path potential_path = working_directory / "assets";
        if (fs::exists(potential_path) && fs::is_directory(potential_path))
            return potential_path.string();

        // Doesn't exist? Check if parent path exists
        if (!working_directory.has_parent_path() ||
            working_directory == working_directory.parent_path())
            break;

        // Move to that parent path
        working_directory = working_directory.parent_path();
    }
    P2REN_FATAL("Failed to find asset directory starting from path {}", base_path);
}

void Application::Run()
{
    SDL_Event event;
    while (m_Window->IsOpen())
    {
        while (SDL_PollEvent(&event))
        {
            // TODO: make event dispatcher handle this
            m_Window->HandleSDLEvents(event);
        }

        m_Renderer->SwapBuffers();
    }
}

std::string_view Application::GetBasePath()
{
    const char* base_path = SDL_GetBasePath();
    P2REN_ASSERT(base_path, "Base path is set to nullptr: {}", SDL_GetError());
    return std::string_view(base_path);
}

} // namespace p2ren
