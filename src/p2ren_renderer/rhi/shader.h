#pragma once

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <string_view>

namespace p2ren {

// TODO: Push constants for
//          - Texture
//          - Framebuffer
class Shader
{
public:
    Shader() = default;
    Shader(std::string_view vertex, std::string_view fragment, std::string_view geometry = "");
    ~Shader();

    Shader(Shader&& other);
    Shader& operator=(Shader&& other);

    Shader(const Shader& other)            = delete;
    Shader& operator=(const Shader& other) = delete;

    void     Bind();
    bool     IsValid() const;
    uint32_t GetID() const { return m_ID; }

    void PushConstant(std::string_view location, int32_t val);
    void PushConstant(std::string_view location, uint32_t val);
    void PushConstant(std::string_view location, float val);

    void PushConstant(std::string_view location, const glm::vec2& val);
    void PushConstant(std::string_view location, const glm::vec3& val);
    void PushConstant(std::string_view location, const glm::vec4& val);

    void PushConstant(std::string_view location, const glm::mat3& val);
    void PushConstant(std::string_view location, const glm::mat4& val);

private:
    uint32_t GetUniformLocation(std::string_view location);

    uint32_t m_ID = 0;
};

// TODO:
class ComputeShader
{
public:
    ComputeShader() = delete;
};

} // namespace p2ren
