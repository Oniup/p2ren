#pragma once

#include <ankerl/unordered_dense.h>
#include <fmt/format.h>
#include <fmt/std.h>

#include <concepts>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "glfwd_core/resource_handle.h"
#include "glfwd_core/utility/error.h"
#include "glfwd_core/utility/type_info.h"

namespace glfwd {

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
class ResourcePool : public IResourcePool
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
        return ResourceHandle<T>(index, m_Generations[index]);
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

        glfwd_ERROR("Cannot query resource '{}' with ID {}, it doesn't exist within pool",
                    TypeInfo<T>::GetName(),
                    handle.m_ID);
        return nullptr;
    }

    const T* QueryResource(const ResourceHandle<T>& handle) const
    {
        if (Contains(handle))
            return &m_Data[handle.m_Index];

        glfwd_ERROR("Cannot query resource '{}' with ID {}, it doesn't exist within pool",
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

    void DestroyResource(const ResourceHandle<T>& handle)
    {
        if (Contains(handle))
        {
            m_Data[handle.m_Index]         = T();
            m_Generations[handle.m_Index] += 1; // Increment resource
            m_FreeSlots.push_back(handle.m_Index);
            return;
        }
        glfwd_WARN("Attempt to destroy resource '{}' with ID {}, it doesn't exist within pool",
                   TypeInfo<T>::GetName(),
                   handle.m_ID);
    }

private:
    std::vector<T> m_Data; // Packed resource data
};

// https://blog.andreiavram.ro/object-has-method-cpp20-concepts/
template <typename T>
concept ResourceHasValidMethod = requires(T resource) {
    { resource.IsValid() } -> std::same_as<bool>;
};

class ResourceManager
{
public:
    ResourceManager(std::string_view path)
        : m_AssetDirectory(path)
    {
    }

    const std::string& GetAssetDirectory() const { return m_AssetDirectory; }

    std::string GetAssetPath(std::string_view path) const
    {
        return fmt::format("{}/{}", m_AssetDirectory, path);
    }

    const char* GetAssetPath(std::string_view path, size_t max_buffer_size, char* buffer) const
    {
        auto result = fmt::format_to_n(buffer, max_buffer_size, "{}/{}", m_AssetDirectory, path);
        *result.out = '\0';
        if (result.size >= max_buffer_size - 1)
            glfwd_ERROR("Buffer (max size {}) provided to format path {}/{} was not large enough, "
                        "result has been cut off",
                        max_buffer_size,
                        m_AssetDirectory,
                        path);
        return buffer;
    }

    template <typename T>
    bool ContainsPool() const
    {
        return m_ResourcePools.contains(TypeInfo<T>::GetUUID());
    }

    template <typename T>
    void InitializePool(size_t initial_capacity = 100, size_t initial_free_slot_capacity = 0)
    {
        static_assert(ResourceHasValidMethod<T>,
                      "Resource must have a defined Valid() for storing in a resource pool");

        if (ContainsPool<T>())
        {
            glfwd_WARN("Attempt to initialize pool for resource {} but it already exists",
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
        if (!resource.IsValid())
        {
            glfwd_ERROR("Cannot add a {} resource that isn't valid on initialization",
                        TypeInfo<T>::GetName());
            return ResourceHandle<T>();
        }

        // If doesn't exist, create a pool
        if (!ContainsPool<T>())
        {
            glfwd_ERROR(
                "Failed to push resource for type: {0}, it pool doesn't exist, need to call "
                "InitializePool<{0}>() on program startup",
                TypeInfo<T>::GetName());
            return ResourceHandle<T>();
        }

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
    std::string                                                            m_AssetDirectory;
    ankerl::unordered_dense::map<uint64_t, std::unique_ptr<IResourcePool>> m_ResourcePools;
};

} // namespace glfwd
