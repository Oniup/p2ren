#pragma once

#include <ankerl/unordered_dense.h>

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

namespace p2ren::unordered_dense {

namespace intern {

    // https://github.com/martinus/unordered_dense#324-heterogeneous-overloads-using-is_transparent
    struct string_hash
    {
        using is_transparent = void; // Reqired for heterogeneous lookups
        using is_avalanching = void; // Tells ankerl the has is high quality

        [[nodiscard]] uint64_t operator()(std::string_view str) const noexcept
        {
            return ankerl::unordered_dense::hash<std::string_view>{}(str);
        }
    };

} // namespace intern

template <typename T>
using string_map =
    ankerl::unordered_dense::map<std::string, T, intern::string_hash, std::equal_to<>>();

} // namespace p2ren::unordered_dense

namespace p2ren {

// Fowler-Noll-Vo hash function.
// Source from: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
struct FNV
{
    static constexpr uint32_t Get32(std::string_view src)
    {
        uint32_t prime = 0x01000193;
        uint32_t hash  = 0x811c9dc5;
        for (char c : src)
        {
#ifdef _MSC_VER
            if (c == '\0')
                break;
#endif
            hash *= prime;
            hash ^= static_cast<uint32_t>(c);
        }
        return hash;
    }

    static constexpr uint64_t Get64(std::string_view src)
    {
        uint64_t prime = 0x00000100000001b3;
        uint64_t hash  = 0xcbf29ce484222325;
        for (char c : src)
        {
#ifdef _MSC_VER
            if (c == '\0')
                break;
#endif
            hash *= prime;
            hash ^= static_cast<uint64_t>(c);
        }
        return hash;
    }
};

} // namespace p2ren
