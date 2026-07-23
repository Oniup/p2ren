#include "glfwd_renderer/resources/model.h"

#include <glm/geometric.hpp>

#include <array>
#include <numbers>

namespace glfwd {

std::array<VertexAttribute, 4> Vertex::GetVertexAttribute()
{
    return std::array<VertexAttribute, 4>{
        VertexAttribute(0, VertexAttribute::Vec3), // Position
        VertexAttribute(0, VertexAttribute::Vec3), // Normal
        VertexAttribute(0, VertexAttribute::Vec3), // Color
        VertexAttribute(0, VertexAttribute::Vec2), // TexCoord
    };
}

Mesh Mesh::GenerateCube(const PhongMaterial& material, glm::vec3 vertex_color)
{
    glm::vec3 color1 = vertex_color == glm::vec3(0.0f) ? glm::vec3(1.0f, 0.0f, 0.0f) : vertex_color;
    glm::vec3 color2 = vertex_color == glm::vec3(0.0f) ? glm::vec3(0.0f, 1.0f, 0.0f) : vertex_color;
    glm::vec3 color3 = vertex_color == glm::vec3(0.0f) ? glm::vec3(0.0f, 0.0f, 1.0f) : vertex_color;
    glm::vec3 color4 = vertex_color == glm::vec3(0.0f) ? glm::vec3(1.0f, 1.0f, 0.0f) : vertex_color;

    std::vector<Vertex> vertices = {
        Vertex{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, color1, {1.0f, 1.0f}},   // top right
        Vertex{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, color2, {1.0f, 0.0f}},  // bottom right
        Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, color3, {0.0f, 0.0f}}, // bottom left
        Vertex{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, color4, {0.0f, 1.0f}},  // top left
    };
    std::vector<uint32_t> indices = {
        // clang-format off
        0, 1, 2, // First triangle
        0, 2, 3, // Second triangle
        // clang-format on
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::GeneratePlane(const PhongMaterial& material, glm::vec3 vertex_color)
{
    glm::vec3 color1 = vertex_color == glm::vec3(0.0f) ? glm::vec3(1.0f, 0.0f, 0.0f) : vertex_color;
    glm::vec3 color2 = vertex_color == glm::vec3(0.0f) ? glm::vec3(0.0f, 1.0f, 0.0f) : vertex_color;
    glm::vec3 color3 = vertex_color == glm::vec3(0.0f) ? glm::vec3(0.0f, 0.0f, 1.0f) : vertex_color;
    glm::vec3 color4 = vertex_color == glm::vec3(0.0f) ? glm::vec3(1.0f, 1.0f, 0.0f) : vertex_color;

    std::vector<Vertex> vertices = {
        // Back face
        Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, color1, {0.0f, 0.0f}},
        Vertex{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, color2, {1.0f, 0.0f}},
        Vertex{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, color3, {1.0f, 1.0f}},
        Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, color4, {0.0f, 1.0f}},

        // Front face
        Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, color1, {0.0f, 0.0f}},
        Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, color2, {1.0f, 0.0f}},
        Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, color3, {1.0f, 1.0f}},
        Vertex{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, color4, {0.0f, 1.0f}},

        // Left face
        Vertex{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, color1, {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, color2, {0.0f, 0.0f}},
        Vertex{{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, color3, {1.0f, 0.0f}},
        Vertex{{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, color4, {1.0f, 1.0f}},

        // Right face
        Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, color1, {0.0f, 1.0f}},
        Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, color2, {0.0f, 0.0f}},
        Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, color3, {1.0f, 0.0f}},
        Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, color4, {1.0f, 1.0f}},

        // Bottom face
        Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, color1, {0.0f, 1.0f}},
        Vertex{{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, color2, {1.0f, 1.0f}},
        Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, color3, {1.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, color4, {0.0f, 0.0f}},

        // Top face
        Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, color1, {0.0f, 1.0f}},
        Vertex{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, color2, {1.0f, 1.0f}},
        Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, color3, {1.0f, 0.0f}},
        Vertex{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, color4, {0.0f, 0.0f}}};

    // clang-format off
    std::vector<uint32_t> indices = {
        // Back face
        0, 2, 1,
        2, 0, 3,
        // Front face
        4, 5, 6,
        6, 7, 4,
        // Left face
        10, 11, 8,
        8, 9, 10,
        // Right face
        14, 12, 15,
        12, 14, 13,
        // Bottom face
        16, 17, 18,
        18, 19, 16,
        // Top face
        20, 22, 21,
        22, 20, 23
    };
    // clang-format on

    return Mesh(vertices, indices, material);
}

Mesh Mesh::GenerateSphere(size_t revolutions_x, size_t revolutions_y, const PhongMaterial& material,
                          glm::vec3 vertex_color)
{
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    vertices.reserve((revolutions_x + 1) * (revolutions_y + 1)); // requires vertices
    indices.reserve(revolutions_x * revolutions_y * 6);          // 6 = quad of triangles

    // Calculate vertices
    // <= due to requiring duplicate vertices at the seams for correct UV mapping
    for (size_t i = 0; i <= revolutions_y; i++)
    {
        float v     = static_cast<float>(i) / revolutions_y; // V of UV
        float theta = v * std::numbers::pi;                  // Half rotation on y to not double up

        for (size_t j = 0; j <= revolutions_x; j++)
        {
            float u   = static_cast<float>(j) / revolutions_x; // U of UV
            float phi = u * 2.0f * std::numbers::pi;           // Full rotation on x

            Vertex vertex;

            // https://danielsieger.com/blog/2021/03/27/generating-spheres.html
            vertex.Position.x = std::sin(theta) * std::cos(phi);
            vertex.Position.y = std::cos(theta);
            vertex.Position.z = std::sin(theta) * std::sin(phi);

            vertex.Normal = glm::normalize(vertex.Position);
            vertex.Color  = vertex_color == glm::vec3(0.0f) ? glm::vec3(u, v, 0.0f) : vertex_color;
            vertex.TexCoord = glm::vec2(u, v);

            vertices.push_back(vertex);
        }
    }

    // Calculate indices, triangles are constructed in clock-wise
    for (size_t i = 0; i < revolutions_y; i++)
    {
        for (size_t j = 0; j < revolutions_x; j++)
        {
            uint32_t current = i * (revolutions_x + 1) + j;
            uint32_t below   = current + (revolutions_x + 1);

            // Triangle 1
            indices.push_back(current);     // Top Left
            indices.push_back(current + 1); // Top Right
            indices.push_back(below);       // Bottom Left
                                            //
            // Triangle 2
            indices.push_back(current + 1); // Top Right
            indices.push_back(below + 1);   // Bottom Right
            indices.push_back(below);       // Bottom Left
        }
    }

    return Mesh(vertices, indices, material);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const PhongMaterial& material, bool dynamic_draw)
    : m_Material(material)
{
    Buffer array_buffer(Buffer::Array, dynamic_draw);
    array_buffer.PushData(vertices.data(), sizeof(Vertex), vertices.size());

    m_Data = VertexArray(Vertex::GetVertexAttribute(), std::move(array_buffer));
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
           const PhongMaterial& material, bool dynamic_draw)
    : m_Material(material)
{
    Buffer array_buffer(Buffer::Array, dynamic_draw);
    Buffer element_buffer(Buffer::Element, dynamic_draw);

    array_buffer.PushData(vertices.data(), sizeof(Vertex), vertices.size());
    element_buffer.PushData(indices.data(), sizeof(uint32_t), indices.size());

    size_t buffer_index = 0;

    m_Data = VertexArray(
        Vertex::GetVertexAttribute(), std::move(array_buffer), std::move(element_buffer));
}

Mesh::Mesh(Mesh&& other)
    : m_Material(other.m_Material),
      m_Data(std::move(other.m_Data))
{
}

Mesh& Mesh::operator=(Mesh&& other)
{
    if (this != &other)
    {
        m_Material = other.m_Material;
        m_Data     = std::move(other.m_Data);
    }
    return *this;
}

bool Mesh::IsValid() const
{
    return m_Data.IsValid();
}

void Mesh::Draw(PrimitiveMode primitive_mode) const
{
    m_Data.Draw(primitive_mode);
}

Model::Model(std::string_view path)
{
}

Model::Model(Mesh&& mesh)
{
}

} // namespace glfwd
