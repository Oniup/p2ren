#pragma once

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <string>
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

    void     Bind();
    bool     Valid() const;
    uint32_t GetInternalID() const { return m_ID; }

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

    // TODO: Implement event dispatcher first before implementing this
#ifdef P2REN_SHADER_HOT_RELOAD
    std::string m_VertexPath;
    std::string m_FragmentPath;
    std::string m_GeometryPath;
#endif
};

// TODO:
class ComputeShader
{
public:
    ComputeShader() = delete;
};

} // namespace p2ren
