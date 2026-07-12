#pragma once

#include <ankerl/unordered_dense.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "p2ren_core/resource_handle.h"
#include "p2ren_core/utility/error.h"
#include "p2ren_core/utility/type_info.h"

namespace p2ren {

class IResourcePool
{
public:
    static constexpr uint16_t InvalidIndex = 0xFFFF;

    virtual ~IResourcePool() = default;

protected:
    std::vector<uint16_t>    m_Generations; // Number of times the slot has been reused
    std::vector<uint16_t>    m_FreeSlots;   // Terminated free slots to fill
    std::vector<std::string> m_Names;       // Human readable resource names
};

template <typename T>
class ResourcePool final : public IResourcePool
{
public:
    ResourcePool()           = default;
    ~ResourcePool() override = default;

    ResourcePool(size_t initial_capacity, size_t initial_free_slot_capacity)
    {
        m_Data.reserve(initial_capacity);
        m_Generations.reserve(initial_capacity);
        m_Names.reserve(initial_capacity);

        if (initial_free_slot_capacity > 0)
            m_FreeSlots.reserve(initial_free_slot_capacity);
    }

    ResourceHandle<T> Push(std::string_view name, T&& resource)
    {
        uint16_t index;
        if (!m_FreeSlots.empty())
        {
            index = m_FreeSlots.back();
            m_FreeSlots.pop_back();

            m_Data[index]  = std::move(resource);
            m_Names[index] = std::string(name);
            // Don't need to increment generation as it gets called on resource termination
        }
        else
        {
            index = static_cast<uint16_t>(m_Data.size());

            m_Data.push_back(std::move(resource));
            m_Names.push_back(std::string(name));
            m_Generations.push_back(0);
        }
        return ResourceHandle(index, m_Generations[index]);
    }

    bool Contains(const ResourceHandle<T>& handle) const
    {
        return handle.IsValid() && handle.m_Index < m_Data.size() &&
               handle.m_Generation == m_Generations[handle.m_Index];
    }

    T* QueryResource(const ResourceHandle<T>& handle)
    {
        if (Contains(handle))
            return &m_Data[handle.m_Index];

        P2REN_ERROR("Cannot query resource '{}' with ID {}, it doesn't exist within pool",
                    TypeInfo<T>::GetName(),
                    handle.m_ID);
        return nullptr;
    }

    const T* QueryResource(const ResourceHandle<T>& handle) const
    {
        if (Contains(handle))
            return &m_Data[handle.m_Index];

        P2REN_ERROR("Cannot query resource '{}' with ID {}, it doesn't exist within pool",
                    TypeInfo<T>::GetName(),
                    handle.m_ID);
        return nullptr;
    }

    /// Finds resource with corresponding name otherwise returns invalid handle. This method is very
    /// slow and should be avoided during runtime.
    ResourceHandle<T> QueryHandle(std::string_view name) const
    {
        for (size_t i = 0; i < m_Names.size(); i++)
        {
            if (name == m_Names[i])
                return ResourceHandle<T>(i, m_Generations[i]);
        }
        return ResourceHandle<T>();
    }

    /// Returns the name of the resource handle, if it doesn't exist, then an empty string
    std::string_view QueryHandleName(const ResourceHandle<T>& handle) const
    {
        if (Contains(handle))
            return m_Names[handle.m_Index];
        return "";
    }

    void Destroy(const ResourceHandle<T>& handle)
    {
        if (Contains(handle))
        {
            m_Data[handle.m_Index]         = T();
            m_Generations[handle.m_Index] += 1; // Increment resource
            m_FreeSlots.push_back(handle.m_Index);
            return;
        }
        P2REN_WARN("Attempt to destroy resource '{}' with ID {}, it doesn't exist within pool",
                   TypeInfo<T>::GetName(),
                   handle.m_ID);
    }

private:
    std::vector<T> m_Data; // Packed resource data
};

class ResourceManager
{
public:
    ResourceManager()  = default;
    ~ResourceManager() = default;

    template <typename T>
    bool ContainsPool()
    {
        return m_ResourcePools.contains(TypeInfo<T>::GetUUID());
    }

    template <typename T>
    void InitializePool(size_t initial_capacity = 100, size_t initial_free_slot_capacity = 0)
    {
        if (ContainsPool<T>())
        {
            P2REN_WARN("Attempt to initialize pool for resource {} but it already exists",
                       TypeInfo<T>::GetName());
            return;
        }
        m_ResourcePools.insert(
            {TypeInfo<T>::GetUUID(),
             std::make_unique<ResourcePool<T>>(initial_capacity, initial_free_slot_capacity)});
    }

    template <typename T>
    ResourceHandle<T> PushResource(std::string_view name, T&& resource)
    {
        // If doesn't exist, create a pool
        if (!ContainsPool<T>())
            InitializePool<T>();

        // Push resource
        constexpr uint64_t type_id = TypeInfo<T>::GetUUID();
        auto               pool = static_cast<ResourcePool<T>*>(m_ResourcePools.at(type_id).get());
        return pool->Push(name, std::move(resource));
    }

    template <typename T>
    T* QueryResource(const ResourceHandle<T>& handle)
    {
        if (!ContainsPool<T>())
            return nullptr;

        auto pool = static_cast<ResourcePool<T>*>(m_ResourcePools.at(TypeInfo<T>::GetUUID()).get());
        return pool->QueryResource(handle);
    }

    template <typename T>
    const T* QueryResource(const ResourceHandle<T>& handle) const
    {
        if (!ContainsPool<T>())
            return nullptr;

        auto pool =
            static_cast<const ResourcePool<T>*>(m_ResourcePools.at(TypeInfo<T>::GetUUID()).get());
        return pool->QueryResource(handle);
    }

    template <typename T>
    std::string_view QueryResourceName(const ResourceHandle<T>& handle) const
    {
        if (!ContainsPool<T>())
            return "";

        auto pool =
            static_cast<const ResourcePool<T>*>(m_ResourcePools.at(TypeInfo<T>::GetUUID()).get());
        return pool->QueryHandleName(handle);
    }

    /// Finds resource with corresponding name otherwise returns invalid handle. This method is very
    /// slow and should be avoided during runtime.
    template <typename T>
    ResourceHandle<T> QueryHandle(std::string_view name) const
    {
        if (!ContainsPool<T>())
            return ResourceHandle<T>();

        auto pool =
            static_cast<const ResourcePool<T>*>(m_ResourcePools.at(TypeInfo<T>::GetUUID()).get());
        return pool->QueryHandle(name);
    }

private:
    ankerl::unordered_dense::map<uint64_t, std::unique_ptr<IResourcePool>> m_ResourcePools;
};

} // namespace p2ren
