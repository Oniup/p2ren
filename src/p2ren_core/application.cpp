#include "p2ren_core/application.h"

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
#include "p2ren_core/transform.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/forward_renderer.h"
#include "p2ren_renderer/resources/mesh.h"
#include "p2ren_renderer/rhi/shader.h"

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

    Buffer ubo_matrices(Buffer::Uniform);

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
