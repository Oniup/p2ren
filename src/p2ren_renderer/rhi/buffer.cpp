#include "p2ren_renderer/rhi/buffer.h"

#include <glad/gl.h>

#include <limits>
#include <vector>

#include "p2ren_core/utility/error.h"

namespace p2ren {
namespace intern {

    int32_t ConvertDataTypeToOpenGL(ShaderVertexDataType type)
    {
        if (type != ShaderVertexDataType::Invalid)
        {
            if (type >= ShaderVertexDataType::Float)
                return GL_FLOAT;
            else if (type >= ShaderVertexDataType::UInt)
                return GL_UNSIGNED_INT;
            else if (type >= ShaderVertexDataType::Int)
                return GL_INT;
        }
        return GL_INVALID_ENUM;
    }

    struct AttributeCreateInfo
    {
        size_t  Stride       = 0;
        size_t  Offset       = 0;
        int32_t CurrentIndex = 0;

        void EnableAttribute(const VertexAttribute& attribute, size_t element_size)
        {
            int32_t data_type = ConvertDataTypeToOpenGL(attribute.Type);

            glEnableVertexAttribArray(CurrentIndex);
            if (data_type == GL_FLOAT)
            {
                glVertexAttribPointer(
                    CurrentIndex,
                    element_size,
                    data_type,
                    attribute.Normalized,
                    Stride,
                    reinterpret_cast<const void*>(static_cast<uintptr_t>(Offset)));
            }
            else
            {
                glVertexAttribIPointer(
                    CurrentIndex,
                    element_size,
                    data_type,
                    Stride,
                    reinterpret_cast<const void*>(static_cast<uintptr_t>(Offset)));
            }

            // Instancing
            if (attribute.Instance)
                glVertexAttribDivisor(CurrentIndex, attribute.Instance);

            Offset += 4 * element_size; // sizeof(float) == sizeof(int) == sizeof(uint) == 4
            CurrentIndex++;             // Increment Attribute ID
        }
    };

} // namespace intern

std::string_view Buffer::ConvertTypeToString(BufferType type)
{
    switch (type)
    {
    case BufferType::Array:   return "Array";
    case BufferType::Element: return "Element";
    case BufferType::Uniform: return "Uniform";
    default:                  return "Invalid";
    }
}

int32_t Buffer::ConvertBufferTypeToRHI(BufferType type)
{
    switch (type)
    {
    case BufferType::Array:   return GL_ARRAY_BUFFER;
    case BufferType::Element: return GL_ELEMENT_ARRAY_BUFFER;
    case BufferType::Uniform: return GL_UNIFORM_BUFFER;
    default:                  return std::numeric_limits<int32_t>::max();
    }
}

Buffer::Buffer(BufferType type, bool is_dynamic)
    : m_Type(type),
      m_IsDynamic(is_dynamic)
{
    glGenBuffers(1, &m_ID);
}

Buffer::~Buffer()
{
    if (m_ID != 0)
    {
        glDeleteBuffers(1, &m_ID);
        m_Type          = BufferType::Invalid;
        m_AllocatedSize = 0;
        m_ID            = 0;
    }
}

Buffer::Buffer(Buffer&& other)
    : m_Type(other.m_Type),
      m_ID(other.m_ID),
      m_AllocatedSize(other.m_AllocatedSize),
      m_IsDynamic(other.m_IsDynamic)
{
    other.m_Type          = BufferType::Invalid;
    other.m_ID            = 0;
    other.m_AllocatedSize = 0;
    other.m_IsDynamic     = false;
}

Buffer& Buffer::operator=(Buffer&& other)
{
    if (m_ID == other.m_ID)
    {
        if (m_ID != 0)
            glDeleteBuffers(1, &m_ID);

        m_Type          = other.m_Type;
        m_ID            = other.m_ID;
        m_AllocatedSize = other.m_AllocatedSize;
        m_IsDynamic     = other.m_IsDynamic;

        other.m_Type          = BufferType::Invalid;
        other.m_ID            = 0;
        other.m_AllocatedSize = 0;
        other.m_IsDynamic     = false;
    }
    return *this;
}

void Buffer::Bind(uint32_t binding_index)
{
    if (m_Type == BufferType::Uniform)
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_index, m_ID);
    else
    {
        int32_t type = ConvertBufferTypeToRHI(m_Type);
        glBindBuffer(type, m_ID);
    }
}

bool Buffer::IsValid() const
{
    return m_ID != 0;
}

void Buffer::PushData(const void* data, size_t size)
{
    int32_t type = ConvertBufferTypeToRHI(m_Type);
    glBindBuffer(type, m_ID);
    glBufferData(type, size, data, m_IsDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    m_AllocatedSize = size;
}

bool Buffer::PushSubData(const void* data, size_t size, size_t offset)
{
    if (size + offset < m_AllocatedSize)
    {
        int32_t type = ConvertBufferTypeToRHI(m_Type);
        glBindBuffer(type, m_ID);
        glBufferSubData(type, offset, size, data);
        return true;
    }

    P2REN_ERROR("Didn't push sub data to {} buffer as the size {} + offset {} = {} is greater than "
                "the allocated size {}",
                ConvertTypeToString(m_Type),
                size,
                offset,
                size + offset,
                m_AllocatedSize);

    return false;
}

VertexAttribute::VertexAttribute(size_t buffer_index, ShaderVertexDataType type, size_t instance,
                                 bool normalized)
    : Type(type),
      BufferIndex(buffer_index),
      Instance(instance),
      Normalized(normalized)
{
}

size_t VertexAttribute::GetShaderTypeByteSize() const
{
    switch (Type)
    {
    case ShaderVertexDataType::Int:   return 1 * sizeof(int);
    case ShaderVertexDataType::IVec2: return 2 * sizeof(int);
    case ShaderVertexDataType::IVec3: return 3 * sizeof(int);
    case ShaderVertexDataType::IVec4: return 4 * sizeof(int);
    case ShaderVertexDataType::UInt:  return 1 * sizeof(float);
    case ShaderVertexDataType::UVec2: return 2 * sizeof(float);
    case ShaderVertexDataType::UVec3: return 3 * sizeof(float);
    case ShaderVertexDataType::UVec4: return 4 * sizeof(float);
    case ShaderVertexDataType::Float: return 1 * sizeof(float);
    case ShaderVertexDataType::Vec2:  return 2 * sizeof(float);
    case ShaderVertexDataType::Vec3:  return 3 * sizeof(float);
    case ShaderVertexDataType::Vec4:  return 4 * sizeof(float);
    case ShaderVertexDataType::Mat2:  return 2 * 2 * sizeof(float);
    case ShaderVertexDataType::Mat3:  return 3 * 3 * sizeof(float);
    case ShaderVertexDataType::Mat4:  return 4 * 4 * sizeof(float);
    default:                          return 0;
    }
}

size_t VertexAttribute::GetShaderTypeElementSize() const
{
    switch (Type)
    {
    case ShaderVertexDataType::Int:   return 1;
    case ShaderVertexDataType::IVec2: return 2;
    case ShaderVertexDataType::IVec3: return 3;
    case ShaderVertexDataType::IVec4: return 4;
    case ShaderVertexDataType::UInt:  return 1;
    case ShaderVertexDataType::UVec2: return 2;
    case ShaderVertexDataType::UVec3: return 3;
    case ShaderVertexDataType::UVec4: return 4;
    case ShaderVertexDataType::Float: return 1;
    case ShaderVertexDataType::Vec2:  return 2;
    case ShaderVertexDataType::Vec3:  return 3;
    case ShaderVertexDataType::Vec4:  return 4;
    case ShaderVertexDataType::Mat2:  return 2; // For Matrices, this represent row/column size
    case ShaderVertexDataType::Mat3:  return 3;
    case ShaderVertexDataType::Mat4:  return 4;
    default:                          return 0;
    }
}

VertexArray::VertexArray(const VertexAttribute* attributes, size_t attribute_count,
                         std::vector<Buffer>&& vertex_buffers, Buffer&& element_buffer)
    : m_ArrayBuffers(std::move(vertex_buffers)),
      m_ElementBuffer(std::move(element_buffer))
{
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
    InitializeAttributes(attributes, attribute_count);

    // Link element buffer to vertex array
    if (m_ElementBuffer.IsValid())
        m_ElementBuffer.Bind();
}

VertexArray::VertexArray(const VertexAttribute* attributes, size_t attribute_count,
                         Buffer&& vertex_buffer, Buffer&& element_buffer)
    : m_ElementBuffer(std::move(element_buffer))
{
    m_ArrayBuffers.push_back(std::move(vertex_buffer));

    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
    InitializeAttributes(attributes, attribute_count);

    // Link element buffer to vertex array
    if (m_ElementBuffer.IsValid())
        m_ElementBuffer.Bind();
}

VertexArray::~VertexArray()
{
    if (m_ID != 0)
    {
        glDeleteVertexArrays(1, &m_ID);
        m_ID = 0;
    }
}

VertexArray::VertexArray(VertexArray&& other)
    : m_ID(other.m_ID),
      m_ArrayBuffers(std::move(other.m_ArrayBuffers)),
      m_ElementBuffer(std::move(other.m_ElementBuffer))
{
    other.m_ID = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other)
{
    if (m_ID != other.m_ID)
    {
        if (m_ID != 0)
            glDeleteVertexArrays(1, &m_ID);

        m_ID            = other.m_ID;
        m_ArrayBuffers  = std::move(other.m_ArrayBuffers);
        m_ElementBuffer = std::move(other.m_ElementBuffer);
    }
    return *this;
}

void VertexArray::Bind()
{
    glBindVertexArray(m_ID);
}

bool VertexArray::IsValid() const
{
    return m_ID != 0 && !m_ArrayBuffers.empty() && m_ArrayBuffers.front().IsValid();
}

void VertexArray::InitializeAttributes(const VertexAttribute* attributes, size_t count)
{
    std::vector<intern::AttributeCreateInfo> attrib_info(m_ArrayBuffers.size());

    // Calculate strides and checks
    for (size_t i = 0; i < count; i++)
    {
        intern::AttributeCreateInfo& info  = attrib_info[attributes[i].BufferIndex];
        info.Stride                       += attributes[i].GetShaderTypeByteSize();

        if (attributes[i].BufferIndex >= m_ArrayBuffers.size())
        {
            P2REN_ERROR(
                "Vertex attribute 'BufferIndex' is larger than the actual Array Buffer count");
            glDeleteVertexArrays(1, &m_ID);
            m_ID = 0;
            return;
        }
    }

    // Create Attributes
    Buffer* prev_buffer = nullptr;
    for (size_t i = 0; i < count; i++)
    {
        intern::AttributeCreateInfo& info   = attrib_info[attributes[i].BufferIndex];
        const VertexAttribute&       attrib = attributes[i];

        // The reason why you should group your buffers
        Buffer* target_buffer = &m_ArrayBuffers[attrib.BufferIndex];
        if (target_buffer != prev_buffer)
        {
            target_buffer->Bind();
            prev_buffer = target_buffer;
        }

        // Enable attribute
        size_t element_size = attrib.GetShaderTypeElementSize();
        if (attrib.Type < ShaderVertexDataType::Mat2)
            info.EnableAttribute(attrib, element_size);
        else
        {
            // Handle types larger than 4 * sizeof(float) aka vec4.
            // https://wikis.khronos.org/opengl/Vertex_Specification#Matrix_attributes
            for (size_t j = 0; j < element_size; j++)
                info.EnableAttribute(attrib, element_size);
        }
    }
}

} // namespace p2ren
