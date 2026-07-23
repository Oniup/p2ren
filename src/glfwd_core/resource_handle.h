#pragma once

#include <cstdint>

#include "glfwd_core/utility/type_info.h"

namespace glfwd {

constexpr static uint32_t InvalidResourceHandleID = 0xFFFFFFFF;

template <typename T>
class ResourcePool;

template <typename T>
class ResourceHandle
{
    friend ResourcePool<T>;

public:
    static constexpr std::string_view TypeName = TypeInfo<T>::GetName();
    static constexpr uint64_t         TypeID   = TypeInfo<T>::GetUUID();

    ResourceHandle()
        : m_ID(InvalidResourceHandleID)
    {
    }

    ResourceHandle(ResourceHandle&& other)                 = default;
    ResourceHandle(const ResourceHandle& other)            = default;
    ResourceHandle& operator=(ResourceHandle&& other)      = default;
    ResourceHandle& operator=(const ResourceHandle& other) = default;

    bool IsValid() const { return m_ID != InvalidResourceHandleID; }
    bool operator==(const ResourceHandle& other) const { return m_ID == other.m_ID; }

private:
    ResourceHandle(uint16_t index, uint16_t generation)
        : m_Index(index),
          m_Generation(generation)
    {
    }

    ResourceHandle(uint32_t id)
        : m_ID(id)
    {
    }

    union
    {
        uint32_t m_ID = InvalidResourceHandleID; /// Combined resource ID
        struct
        {
            uint16_t m_Index;      // Resource pool index
            uint16_t m_Generation; // Starts at 0 but increments when slot is reused
        };
    };
};
} // namespace glfwd
