#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <string_view>

namespace glfwd {

struct Color
{
    static glm::vec3 NormalizeRGB(uint8_t r, uint8_t g, uint8_t b);
    static glm::vec4 NormalizeRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    static glm::vec3 ConvertFromtHexRGB(std::string_view rgb_hex);
    static glm::vec4 ConvertFromtHexRGBA(std::string_view rgba_hex);

    static glm::vec3 ConvertHexToColorRGB(uint32_t rgb_hex);
    static glm::vec4 ConvertFromHexRGBA(uint32_t rgba_hex);
};

} // namespace glfwd
