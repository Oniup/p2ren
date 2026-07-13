#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

namespace p2ren {

enum class BufferType
{
    Invalid,
    Array,
    Element,
    Uniform,
};

class Buffer
{
public:
    static std::string_view ConvertTypeToString(BufferType type);
    static int32_t          ConvertBufferTypeToRHI(BufferType type);

    Buffer() = default;
    Buffer(BufferType type, bool is_dynamic = false);
    ~Buffer();

    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);

    Buffer(const Buffer& other)            = delete;
    Buffer& operator=(const Buffer& other) = delete;

    void Bind(uint32_t binding_index = 0);
    bool IsValid() const;

    BufferType GetType() const { return m_Type; }
    uint32_t   GetID() const { return m_ID; }
    size_t     GetAllocatedSize() const { return m_AllocatedSize; }
    bool       IsDynamic() const { return m_IsDynamic; }

    void PushData(const void* data, size_t size);
    bool PushSubData(const void* data, size_t size, size_t offset);

private:
    BufferType m_Type          = BufferType::Invalid;
    uint32_t   m_ID            = 0;
    size_t     m_AllocatedSize = 0;
    bool       m_IsDynamic     = false;
};

enum class ShaderVertexDataType
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

struct VertexAttribute
{
    ShaderVertexDataType Type        = ShaderVertexDataType::Float;
    size_t               BufferIndex = 0;     // Target buffer index who's data to read
    size_t               Instance    = 0;     // if > 0 then sets glVertexAttribDivisor(i, Instance)
    bool                 Normalized  = false; // If data entry is normalized or not

    VertexAttribute(size_t buffer_index, ShaderVertexDataType type, size_t instance = 0,
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

    void Bind();
    bool IsValid() const;

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
