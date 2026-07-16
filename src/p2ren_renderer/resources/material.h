#pragma once

#include <glm/ext/vector_float3.hpp>

#include <variant>

#include "p2ren_core/resource_handle.h"
#include "p2ren_renderer/rhi/texture.h"

namespace p2ren {

class Shader;

struct MaterialColor
{
    glm::vec3               Color;
    ResourceHandle<Texture> Image;

    MaterialColor() = default;
    MaterialColor(glm::vec3 color);
    MaterialColor(const ResourceHandle<Texture>& image);
    MaterialColor(glm::vec3 color, const ResourceHandle<Texture>& image);
};

struct Material
{
    using TextureOrScaler = std::variant<float, ResourceHandle<Texture>>;

    MaterialColor BaseColor;
    struct
    {
        MaterialColor                                Color;
        std::variant<float, ResourceHandle<Texture>> Shinniness = 1.0f;
    } Specular;
    struct
    {
        MaterialColor Color;
        float         Scaler = 1.0f;
    };
    MaterialColor Ambient;

    void PushUniformsToShader(Shader& shader);
};

} // namespace p2ren
