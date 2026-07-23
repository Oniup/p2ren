#include "glfwd_example/game.h"

#include <filesystem>

#include "glfwd_core/resource_manager.h"
#include "glfwd_renderer/render_queue.h"

namespace glfwd_example {

std::string Game::FindAssetDirectory()
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
    GLFWD_FATAL("Failed to find asset directory starting from path {}", base_path);
}

void Game::OnInitialize()
{
    m_Transform    = glfwd::Transform{};
    m_ShaderHandle = m_ResourceManager->QueryHandle<glfwd::Shader>("Basic Blinn-Phong");

    glfwd::TextureCreateInfo default_create_info{
        .MinFilter  = glfwd::TextureFilter::Linear,
        .MagFilter  = glfwd::TextureFilter::Linear,
        .Anisotropy = 16,
    };

    m_Mesh = glfwd::Mesh::GeneratePlane(
        glfwd::PhongMaterial{
            .Diffuse = m_ResourceManager->PushResource(
                "Container Color",
                glfwd::Texture(m_ResourceManager->GetAssetPath("textures/container/color.png"),
                               default_create_info)),
            .Specular = m_ResourceManager->PushResource(
                "Container Specular",
                glfwd::Texture(m_ResourceManager->GetAssetPath("textures/container/specular.png"),
                               default_create_info)),
        },
        glm::vec3(0.0f, 0.0f, 0.0f));

    glfwd::Shader* shader = m_ResourceManager->QueryResource<glfwd::Shader>(m_ShaderHandle);
    glfwd::PhongMaterial::SetupShaderMaterial(shader, true);
}

void Game::OnEvent(const SDL_Event& event)
{
}

void Game::OnUpdate(const glfwd::Timestep& timestep)
{
}

void Game::OnLateUpdate(const glfwd::Timestep& timestep)
{
}

void Game::SubmitToRenderQueue(glfwd::RenderQueue* render_queue)
{
    glfwd::Shader* shader = m_ResourceManager->QueryResource(m_ShaderHandle);
    shader->Bind();
    shader->PushConstant("u_Model", m_Transform.CreateModelMatrix());
    m_Mesh.GetMaterial().PushConstantsToShader(shader, m_ResourceManager);

    m_Mesh.Draw(glfwd::PrimitiveMode::Triangles);
}

} // namespace glfwd_example
