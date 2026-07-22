#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include "p2ren_core/resource_handle.h"
#include "p2ren_renderer/rhi/texture.h"

namespace p2ren {

namespace intern {
    void InitializeDefaultImageAssignment(const ResourceHandle<Texture>& white_texture);
}

class Shader;
class ResourceManager;

struct PhongMaterial
{
    struct ColorAttribute
    {
        glm::vec3               Color;
        ResourceHandle<Texture> TextureHandle;

        ColorAttribute();
        ColorAttribute(glm::vec3 color);
        ColorAttribute(const ResourceHandle<Texture>& texture);
        ColorAttribute(glm::vec3 color, const ResourceHandle<Texture>& texture);
    };

    static void SetupShaderMaterial(const Shader* shader, bool bind_shader = false);

    ColorAttribute Diffuse;
    ColorAttribute Specular;
    ColorAttribute Emission;

    glm::vec2 TilingFactor = glm::vec2(1.0f);
    float     Shininess    = 1.0f;

    void PushConstantsToShader(const Shader* shader, ResourceManager* resources);
};

} // namespace p2ren
