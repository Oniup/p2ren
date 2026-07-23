#include "glfwd_example/game.h"

#include "glfwd_core/resource_manager.h"
#include "glfwd_renderer/render_queue.h"
#include "glfwd_renderer/resources/material.h"

namespace glfwd_example {

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
