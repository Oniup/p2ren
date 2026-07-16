#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

#include "p2ren_renderer/resources/material.h"
#include "p2ren_renderer/rhi/buffer.h"

namespace p2ren {

// TODO: Support instancing vertex array attributes
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Color;
    glm::vec2 TexCoord;

    static std::array<VertexAttribute, 4> GetVertexAttribute();
    static std::array<VertexAttribute, 4> GetInstancedVertexAttribute() = delete ("incomplete");
};

class Mesh
{
public:
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GenerateCube(const Material& material     = {},
                             glm::vec3       vertex_color = glm::vec3(1.0f));
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GeneratePlane(const Material& material     = {},
                              glm::vec3       vertex_color = glm::vec3(1.0f));
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GenerateSphere(size_t revolutions_x = 20, size_t revolutions_y = 20,
                               const Material& material     = {},
                               glm::vec3       vertex_color = glm::vec3(1.0f));

    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const Material& material = {},
         bool dynamic_draw = false);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
         const Material& material = {}, bool dynamic_draw = false);

    bool IsValid() const;

    void Draw() const;

    Material&       GetMaterial() { return m_Material; }
    const Material& GetMaterial() const { return m_Material; }

private:
    Material    m_Material;
    VertexArray m_Data;
};

} // namespace p2ren
