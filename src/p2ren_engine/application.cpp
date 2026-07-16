#include "p2ren_engine/application.h"

// TODO: SDL and glad should be removed later
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <fmt/base.h>
#include <glad/gl.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <string_view>

#include "p2ren_core/resource_manager.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/forward_renderer.h"
#include "p2ren_renderer/resources/mesh.h"
#include "p2ren_renderer/rhi/shader.h"
#include "p2ren_world/components/transform.h"

namespace p2ren {

Application* Application::s_Application = nullptr;

// clang-format off
constexpr std::array<unsigned, 6> PlaneIndices = {
    0, 1, 2, // First triangle
    0, 2, 3, // Second triangle
};
// clang-format on

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
    m_ResourceManager = new ResourceManager(
        create_info.AssetDirectory.empty() ? FindAssetDirectory() : create_info.AssetDirectory);

    // Initialize window and backend contexts
    m_Renderer = new ForwardRenderer(m_ResourceManager, create_info.Renderer);
    m_Window   = new Window(create_info.Window);

    // Initialize forward renderer and RHI context
    m_Renderer->InitializeBackend(m_Window);
    m_Renderer->InitializeResources();
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
    Mesh           mesh        = Mesh::GeneratePlane(Material{}, glm::vec3(0.0f, 0.0f, -0.0f));
    Transform      transform   = Transform{.Position = glm::vec3(0.0f)};
    ResourceHandle flat_shader = m_ResourceManager->QueryHandle<Shader>("Flat Color");

    SDL_Event event;
    while (m_Window->IsOpen())
    {
        while (SDL_PollEvent(&event))
        {
            // TODO: make event dispatcher handle this
            m_Window->HandleSDLEvents(event);
        }

        glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Shader* shader = m_ResourceManager->QueryResource(flat_shader);
        shader->Bind();
        shader->PushConstant("u_Model", transform.CreateModelMatrix());
        mesh.Draw();

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
