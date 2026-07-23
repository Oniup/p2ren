#include "glfwd_renderer/rhi/buffer.h"

#include <glad/gl.h>

#include <limits>
#include <vector>

#include "glfwd_core/utility/error.h"
#include "glfwd_renderer/rhi/context.h"

namespace glfwd {
namespace {

    int32_t ConvertDataTypeToOpenGL(VertexAttribute::DataType type)
    {
        if (type != VertexAttribute::Invalid)
        {
            if (type >= VertexAttribute::Float)
                return GL_FLOAT;
            else if (type >= VertexAttribute::UInt)
                return GL_UNSIGNED_INT;
            else if (type >= VertexAttribute::Int)
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

} // namespace

std::string_view Buffer::ConvertTypeToString(Type type)
{
    switch (type)
    {
    case Buffer::Array:   return "Array";
    case Buffer::Element: return "Element";
    case Buffer::Uniform: return "Uniform";
    default:              return "Invalid";
    }
}

int32_t Buffer::ConvertTypeToRHI(Type type)
{
    switch (type)
    {
    case Buffer::Array:   return GL_ARRAY_BUFFER;
    case Buffer::Element: return GL_ELEMENT_ARRAY_BUFFER;
    case Buffer::Uniform: return GL_UNIFORM_BUFFER;
    default:              return std::numeric_limits<int32_t>::max();
    }
}

Buffer::Buffer(Type type, bool is_dynamic)
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
        m_Type = Buffer::Invalid;
        m_Size = 0;
        m_ID   = 0;
    }
}

Buffer::Buffer(Buffer&& other)
    : m_Type(other.m_Type),
      m_ID(other.m_ID),
      m_Size(other.m_Size),
      m_IsDynamic(other.m_IsDynamic)
{
    other.m_Type      = Type::Invalid;
    other.m_ID        = 0;
    other.m_Size      = 0;
    other.m_IsDynamic = false;
}

Buffer& Buffer::operator=(Buffer&& other)
{
    if (this != &other)
    {
        if (m_ID != 0)
            glDeleteBuffers(1, &m_ID);

        m_Type      = other.m_Type;
        m_ID        = other.m_ID;
        m_Size      = other.m_Size;
        m_IsDynamic = other.m_IsDynamic;

        other.m_Type      = Type::Invalid;
        other.m_ID        = 0;
        other.m_Size      = 0;
        other.m_IsDynamic = false;
    }
    return *this;
}

void Buffer::Bind(uint32_t binding_index)
{
    if (m_Type == Type::Uniform)
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_index, m_ID);
    else
    {
        int32_t type = ConvertTypeToRHI(m_Type);
        glBindBuffer(type, m_ID);
    }
}

bool Buffer::IsValid() const
{
    return m_ID != 0;
}

void Buffer::PushData(const void* data, size_t stride_size, size_t size)
{
    int32_t type = ConvertTypeToRHI(m_Type);
    glBindBuffer(type, m_ID);
    glBufferData(type, stride_size * size, data, m_IsDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    m_Size          = size;
    m_AllocatedSize = stride_size * size;
}

bool Buffer::PushSubData(const void* data, size_t stride_size, size_t size, size_t offset)
{
    if (size + offset < m_Size)
    {
        int32_t type = ConvertTypeToRHI(m_Type);
        glBindBuffer(type, m_ID);
        glBufferSubData(type, offset, stride_size * size, data);
        return true;
    }

    GLFWD_ERROR("Didn't push sub data to {} buffer as the size {} + offset {} = {} is greater than "
                "the allocated size {}",
                ConvertTypeToString(m_Type),
                size,
                offset,
                stride_size * size + offset,
                m_Size);

    return false;
}

VertexAttribute::VertexAttribute(size_t buffer_index, DataType type, size_t instance,
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
    case VertexAttribute::Int:   return 1 * sizeof(int);
    case VertexAttribute::IVec2: return 2 * sizeof(int);
    case VertexAttribute::IVec3: return 3 * sizeof(int);
    case VertexAttribute::IVec4: return 4 * sizeof(int);
    case VertexAttribute::UInt:  return 1 * sizeof(float);
    case VertexAttribute::UVec2: return 2 * sizeof(float);
    case VertexAttribute::UVec3: return 3 * sizeof(float);
    case VertexAttribute::UVec4: return 4 * sizeof(float);
    case VertexAttribute::Float: return 1 * sizeof(float);
    case VertexAttribute::Vec2:  return 2 * sizeof(float);
    case VertexAttribute::Vec3:  return 3 * sizeof(float);
    case VertexAttribute::Vec4:  return 4 * sizeof(float);
    case VertexAttribute::Mat2:  return 2 * 2 * sizeof(float);
    case VertexAttribute::Mat3:  return 3 * 3 * sizeof(float);
    case VertexAttribute::Mat4:  return 4 * 4 * sizeof(float);
    default:                     return 0;
    }
}

size_t VertexAttribute::GetShaderTypeElementSize() const
{
    switch (Type)
    {
    case VertexAttribute::Int:   return 1;
    case VertexAttribute::IVec2: return 2;
    case VertexAttribute::IVec3: return 3;
    case VertexAttribute::IVec4: return 4;
    case VertexAttribute::UInt:  return 1;
    case VertexAttribute::UVec2: return 2;
    case VertexAttribute::UVec3: return 3;
    case VertexAttribute::UVec4: return 4;
    case VertexAttribute::Float: return 1;
    case VertexAttribute::Vec2:  return 2;
    case VertexAttribute::Vec3:  return 3;
    case VertexAttribute::Vec4:  return 4;
    case VertexAttribute::Mat2:  return 2; // For Matrices, this represent row/column size
    case VertexAttribute::Mat3:  return 3;
    case VertexAttribute::Mat4:  return 4;
    default:                     return 0;
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
    if (this != &other)
    {
        if (m_ID != 0)
            glDeleteVertexArrays(1, &m_ID);

        m_ID            = other.m_ID;
        m_ArrayBuffers  = std::move(other.m_ArrayBuffers);
        m_ElementBuffer = std::move(other.m_ElementBuffer);

        other.m_ID = 0;
    }
    return *this;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_ID);
}

void VertexArray::Draw(PrimitiveMode mode, uint32_t offset, uint32_t size) const
{
    int32_t gl_primitive = OpenGLContext::ConvertPrimitiveModeToOpenGL(mode);

    glBindVertexArray(m_ID);
    if (m_ElementBuffer.IsValid())
    {
        size = size == 0 ? m_ElementBuffer.GetSize() : size;
        GLFWD_ASSERT(size <= m_ElementBuffer.GetSize(),
                     "Size cannot exceed the element buffers size {}",
                     size,
                     m_ElementBuffer.GetSize());
        GLFWD_ASSERT(offset <= size, "Offset {} cannot exceed the size {}", offset, size);
        glDrawElements(gl_primitive,
                       size,
                       GL_UNSIGNED_INT,
                       reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
    }
    else
    {
        if (size == 0)
        {
            size_t size = 0;
            for (const Buffer& buffer : m_ArrayBuffers)
                size += buffer.GetSize();
        }
#ifndef NDEBUG
        else
        {
            size_t total_size = 0;
            for (const Buffer& buffer : m_ArrayBuffers)
                total_size += buffer.GetSize();
            GLFWD_ASSERT(size <= total_size,
                         "Size cannot exceed the collective array buffers ({} total) size {}",
                         size,
                         m_ArrayBuffers.size(),
                         total_size);
        }
#endif

        GLFWD_ASSERT(offset >= size, "Offset {} cannot exceed the size {}", offset, size);
        glDrawArrays(gl_primitive, offset, size);
    }
}

bool VertexArray::IsValid() const
{
    if (m_ID != 0 && !m_ArrayBuffers.empty())
    {
        for (const Buffer& buffer : m_ArrayBuffers)
        {
            if (!buffer.IsValid())
                return false;
        }
        return true;
    }
    return false;
}

void VertexArray::InitializeAttributes(const VertexAttribute* attributes, size_t count)
{
    std::vector<AttributeCreateInfo> attrib_info(m_ArrayBuffers.size());

    // Calculate strides and checks
    for (size_t i = 0; i < count; i++)
    {
        AttributeCreateInfo& info  = attrib_info[attributes[i].BufferIndex];
        info.Stride               += attributes[i].GetShaderTypeByteSize();

        if (attributes[i].BufferIndex >= m_ArrayBuffers.size())
        {
            GLFWD_ERROR(
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
        AttributeCreateInfo&   info   = attrib_info[attributes[i].BufferIndex];
        const VertexAttribute& attrib = attributes[i];

        // The reason why you should group your buffers
        Buffer* target_buffer = &m_ArrayBuffers[attrib.BufferIndex];
        if (target_buffer != prev_buffer)
        {
            target_buffer->Bind();
            prev_buffer = target_buffer;
        }

        // Enable attribute
        size_t element_size = attrib.GetShaderTypeElementSize();
        if (attrib.Type < VertexAttribute::Mat2)
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

} // namespace glfwd
