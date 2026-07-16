#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

#include "p2ren_renderer/draw_modes.h"

namespace p2ren {

class Buffer
{
public:
    enum class Type
    {
        Invalid,
        Array,
        Element,
        Uniform,
    };

    using enum Type;

    static std::string_view ConvertTypeToString(Type type);
    static int32_t          ConvertTypeToRHI(Type type);

    Buffer() = default;
    Buffer(Type type, bool is_dynamic = false);
    ~Buffer();

    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);

    Buffer(const Buffer& other)            = delete;
    Buffer& operator=(const Buffer& other) = delete;

    void Bind(uint32_t binding_index = 0);
    bool IsValid() const;

    Type     GetType() const { return m_Type; }
    uint32_t GetID() const { return m_ID; }
    size_t   GetSize() const { return m_Size; }
    size_t   GetAllocatedSize() const { return m_AllocatedSize; }
    bool     IsDynamic() const { return m_IsDynamic; }

    void PushData(const void* data, size_t stride_size, size_t size);
    bool PushSubData(const void* data, size_t stride_size, size_t size, size_t offset);

private:
    Type     m_Type          = Type::Invalid;
    uint32_t m_ID            = 0;
    size_t   m_Size          = 0;
    size_t   m_AllocatedSize = 0;
    bool     m_IsDynamic     = false;
};

struct VertexAttribute
{
    enum class DataType
    {
        Invalid,
        Int,
        IVec2,
        IVec3,
        IVec4,
        UInt,
        UVec2,
        UVec3,
        UVec4,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat2,
        Mat3,
        Mat4,
    };
    using enum DataType;

    DataType Type        = VertexAttribute::Float;
    size_t   BufferIndex = 0;     // Target buffer index who's data to read
    size_t   Instance    = 0;     // if > 0 then sets glVertexAttribDivisor(i, Instance)
    bool     Normalized  = false; // If data entry is normalized or not

    VertexAttribute(size_t buffer_index, DataType type, size_t instance = 0,
                    bool normalized = false);

    size_t GetShaderTypeByteSize() const;
    size_t GetShaderTypeElementSize() const;
};

class VertexArray
{
public:
    VertexArray() = default;
    VertexArray(const VertexAttribute* attributes, size_t attribute_count,
                std::vector<Buffer>&& vertex_buffers, Buffer&& element_buffer = Buffer());
    VertexArray(const VertexAttribute* attributes, size_t attribute_count, Buffer&& vertex_buffer,
                Buffer&& element_buffer = Buffer());
    ~VertexArray();

    template <size_t AttributeCount>
    VertexArray(const std::array<VertexAttribute, AttributeCount>& attributes,
                std::vector<Buffer>&& vertex_buffers, Buffer&& element_buffer = Buffer())
        : VertexArray(attributes.data(), attributes.size(), std::move(vertex_buffers),
                      std::move(element_buffer))
    {
    }

    template <size_t AttributeCount>
    VertexArray(const std::array<VertexAttribute, AttributeCount>& attributes,
                Buffer&& vertex_buffer, Buffer&& element_buffer = Buffer())
        : VertexArray(attributes.data(), attributes.size(), std::move(vertex_buffer),
                      std::move(element_buffer))
    {
    }

    VertexArray(VertexArray&& other);
    VertexArray& operator=(VertexArray&& other);

    VertexArray(const VertexArray& other)            = delete;
    VertexArray& operator=(const VertexArray& other) = delete;

    bool IsValid() const;

    void Bind() const;
    void Draw(PrimitiveMode mode = PrimitiveMode::Triangles, uint32_t offset = 0,
              uint32_t size = 0) const;

    std::vector<Buffer>& GetArrayBuffers() { return m_ArrayBuffers; }
    Buffer&              GetElementBuffer() { return m_ElementBuffer; }

    const std::vector<Buffer>& GetArrayBuffers() const { return m_ArrayBuffers; }
    const Buffer&              GetElementBuffer() const { return m_ElementBuffer; }

private:
    void InitializeAttributes(const VertexAttribute* attributes, size_t count);

    uint32_t            m_ID;
    std::vector<Buffer> m_ArrayBuffers;
    Buffer              m_ElementBuffer;
};

} // namespace p2ren
