#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include <type_traits>

#include "p2ren_core/hash.h"

namespace p2ren::intern {

template <typename T>
struct remove_all_pointers
{
    using type = T;
};

template <typename T>
struct remove_all_pointers<T*>
{
    using type = typename remove_all_pointers<T>::type;
};

template <typename T>
struct stripped_type
{
    using remove_extents_type   = std::remove_all_extents_t<T>;
    using remove_reference_type = std::remove_reference_t<remove_extents_type>;
    using remove_pointers_type  = typename remove_all_pointers<T>::type;
    using type                  = std::remove_cvref_t<remove_pointers_type>;
};

#ifdef _MSC_VER
static constexpr std::array<std::string_view, 3> MsvcPrefixNames = {
    "struct ",
    "class ",
    "enum ",
};
#endif

template <typename T>
constexpr std::string_view GetRawTypeName()
{
#if defined(__clang__) || defined(__GNUC__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
#    error "Unsupported compiler"
#endif
}

constexpr size_t GetRawPrefixLength()
{
    std::string_view wrapped(GetRawTypeName<void>());
    return wrapped.find("void");
}

constexpr size_t GetRawSuffixLength()
{
    std::string_view wrapped(GetRawTypeName<void>());
    size_t           prefix = GetRawPrefixLength();
    return wrapped.length() - prefix - 4;
}

template <typename T>
constexpr std::string_view GetUnwrappedTypeName()
{
    std::string_view wrapped(GetRawTypeName<T>());
    size_t           prefix = GetRawPrefixLength();
    size_t           suffix = GetRawSuffixLength();
    size_t           length = wrapped.length() - prefix - suffix;
    return wrapped.substr(prefix, length);
}

template <typename T>
constexpr size_t GetCleanedSize()
{
    std::string_view name = GetUnwrappedTypeName<T>();
    size_t           size = 0;

    for (size_t i = 0; i < name.size(); i++)
    {
        std::string_view substr = name.substr(i);
        if (substr.starts_with(' '))
        {
            size++;
            continue;
        }

#ifdef _MSC_VER
        for (std::string_view prefix : MsvcPrefixNames)
        {
            if (substr.starts_with(prefix))
            {
                size += prefix.size() - 1;
                i    += prefix.size() - 1;
                break;
            }
        }
#endif
    }

    return name.size() - size;
}

template <typename T, size_t N>
constexpr std::array<char, N> GetCleanedTypeNameArray()
{
    std::array<char, N> cleaned{};
    std::string_view    unwrapped = GetUnwrappedTypeName<T>();

    size_t j = 0;
    for (size_t i = 0; i < unwrapped.size(); i++)
    {
        std::string_view substr = unwrapped.substr(i);

        // Skip spaces
        if (substr.starts_with(' '))
            continue;

#ifdef _MSC_VER
        bool foundPrefix = false;
        for (std::string_view prefix : MsvcPrefixNames)
        {
            if (substr.starts_with(prefix))
            {
                i           += prefix.size() - 1;
                foundPrefix  = true;
                break;
            }
        }

        if (foundPrefix)
            continue;
#endif
        cleaned[j++] = unwrapped[i];
    }

    return cleaned;
}

} // namespace p2ren::intern

namespace p2ren {

template <typename T>
struct TypeInfo
{
    static constexpr std::string_view GetName()
    {
        const std::array<char, s_CleanedSize>& name = TypeInfo<T>::s_Cleaned;
        return std::string_view(name.data(), name.size());
    }

    static constexpr uint64_t GetHash()
    {
        std::string_view name = GetName();
        return FNV::Get32(name);
    }

private:
    static constexpr size_t s_CleanedSize = intern::GetCleanedSize<T>();

    static constexpr std::array<char, s_CleanedSize> s_Cleaned =
        intern::GetCleanedTypeNameArray<T, s_CleanedSize>();
};

} // namespace p2ren
