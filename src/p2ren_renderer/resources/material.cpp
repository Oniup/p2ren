#include "p2ren_renderer/resources/material.h"

namespace p2ren {

MaterialColor::MaterialColor(glm::vec3 color)
    : Color(color),
      Image(ResourceHandle<Texture>{})
{
}

MaterialColor::MaterialColor(const ResourceHandle<Texture>& image)
    : Color(1.0),
      Image(ResourceHandle<Texture>{})
{
}

MaterialColor::MaterialColor(glm::vec3 color, const ResourceHandle<Texture>& image)
    : Color(color),
      Image(image)
{
}

void Material::PushUniformsToShader(Shader& shader)
{
}

} // namespace p2ren
