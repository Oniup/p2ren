#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

#include "glfwd_renderer/draw_modes.h"
#include "glfwd_renderer/resources/material.h"
#include "glfwd_renderer/rhi/buffer.h"

namespace glfwd {

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
    static Mesh GenerateCube(const PhongMaterial& material     = {},
                             glm::vec3            vertex_color = glm::vec3(1.0f));
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GeneratePlane(const PhongMaterial& material     = {},
                              glm::vec3            vertex_color = glm::vec3(1.0f));
    /// Set vertex_color to glm::vec3(0.0f) to apply the rainbow UV vertex colors
    static Mesh GenerateSphere(size_t revolutions_x = 20, size_t revolutions_y = 20,
                               const PhongMaterial& material     = {},
                               glm::vec3            vertex_color = glm::vec3(1.0f));

    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const PhongMaterial& material = {},
         bool dynamic_draw = false);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
         const PhongMaterial& material = {}, bool dynamic_draw = false);

    Mesh(Mesh&& other);
    Mesh& operator=(Mesh&& other);

    Mesh(const Mesh& other)            = delete;
    Mesh& operator=(const Mesh& other) = delete;

    bool IsValid() const;

    void Draw(PrimitiveMode primitive_mode) const;

    PhongMaterial&       GetMaterial() { return m_Material; }
    const PhongMaterial& GetMaterial() const { return m_Material; }

private:
    PhongMaterial m_Material;
    VertexArray   m_Data;
};

class Model
{
public:
    Model() = default;
    Model(std::string_view path);
    Model(Mesh&& mesh);

private:
    std::vector<Mesh> m_Meshes;
};

} // namespace glfwd
