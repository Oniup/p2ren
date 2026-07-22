#include "p2ren_renderer/resources/material.h"

#include "p2ren_core/resource_manager.h"
#include "p2ren_renderer/rhi/shader.h"

namespace p2ren {

namespace intern {

    ResourceHandle<Texture> s_DefaultWhiteTexture = {};

    void InitializeDefaultImageAssignment(const ResourceHandle<Texture>& white_texture)
    {
        intern::s_DefaultWhiteTexture = white_texture;
    }

} // namespace intern

PhongMaterial::ColorAttribute::ColorAttribute()
    : Color(glm::vec3(1.0f)),
      TextureHandle(intern::s_DefaultWhiteTexture)
{
}

PhongMaterial::ColorAttribute::ColorAttribute(glm::vec3 color)
    : Color(color),
      TextureHandle(intern::s_DefaultWhiteTexture)
{
}

PhongMaterial::ColorAttribute::ColorAttribute(const ResourceHandle<Texture>& texture)
    : Color(1.0),
      TextureHandle(texture)
{
}

PhongMaterial::ColorAttribute::ColorAttribute(glm::vec3                      color,
                                              const ResourceHandle<Texture>& texture)
    : Color(color),
      TextureHandle(texture)
{
}

void PhongMaterial::SetupShaderMaterial(const Shader* shader, bool bind_shader)
{
    if (bind_shader)
        shader->Bind();

    shader->PushConstant("u_Material.Diffuse.Texture", 0);
    shader->PushConstant("u_Material.Specular.Texture", 1);
    shader->PushConstant("u_Material.Emission.Texture", 2);
}

void PhongMaterial::PushConstantsToShader(const Shader* shader, ResourceManager* resources)
{
    // Flat Colors
    shader->PushConstant("u_Material.Diffuse.Color", Diffuse.Color);
    shader->PushConstant("u_Material.Specular.Color", Specular.Color);
    shader->PushConstant("u_Material.Emission.Color", Emission.Color);

    // Textures
    shader->PushConstant(resources->QueryResource(Diffuse.TextureHandle), 0);
    shader->PushConstant(resources->QueryResource(Specular.TextureHandle), 1);
    shader->PushConstant(resources->QueryResource(Emission.TextureHandle), 2);

    // Other properties
    shader->PushConstant("u_Material.Shininess", Shininess);
    shader->PushConstant("u_Material_TilingFactor", TilingFactor);
}

} // namespace p2ren
