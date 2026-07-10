#pragma once

#include <fmt/core.h>
#include <fmt/std.h> // IWYU pragma: export

#include <cstdio>
#include <string_view>

namespace p2ren {

enum class ErrorSeverity
{
    Low,    // Info
    Medium, // Warning
    High,   // Error
    Fatal,  // Asserts/Fatal
};

/// Set lowest filter target, if set to ErrorSeverity::High, all info and warning logs will be
/// culled
static void Error_SetFilterLower(ErrorSeverity severity);

namespace intern {

    /// Returns true if severity is a smalller value than the set lowest filter. Update filter
    /// threshold by calling Error_SetFilterLower()
    bool Error_ShouldFilter(ErrorSeverity severity);

    /// Prints the header of the error message
    void Error_PrintErrorHeaderMessage(ErrorSeverity severity, std::string_view file,
                                       std::string_view function, int line,
                                       std::string_view expression);

    /// Prints error message
    void Error_PrintAssertErrorMessage(ErrorSeverity severity, std::string_view file,
                                       std::string_view function, int line);

    /// Uses Error_PrintErrorHeaderMessage and Error_PrintAssertErrorMessage to format an assert
    /// message utilizing libfmt for string manipulation
    void Error_OnAssert(std::string_view file, std::string_view function, int line,
                        std::string_view expression);

    /// Uses Error_PrintErrorHeaderMessage and Error_PrintAssertErrorMessage to format an assert
    /// message utilizing libfmt for string manipulation
    template <typename... Args>
    void Error_OnAssert(std::string_view file, std::string_view function, int line,
                        std::string_view expression, fmt::format_string<Args...> format,
                        Args&&... args)
    {
        Error_PrintErrorHeaderMessage(ErrorSeverity::High, file, function, line, expression);

        fmt::println(stderr, format, std::forward<Args>(args)...);
        std::fflush(stderr);
    }

    /// Uses Error_PrintErrorHeaderMessage and Error_PrintAssertErrorMessage to format an error
    /// message utilizing libfmt for string manipulation
    template <typename... Args>
    void Error_OnLog(ErrorSeverity severity, std::string_view file, std::string_view function,
                     int line, fmt::format_string<Args...> format, Args&&... args)
    {
        if (Error_ShouldFilter(severity))
            return;

        Error_PrintAssertErrorMessage(severity, file, function, line);

        fmt::println(stderr, format, std::forward<Args>(args)...);
        std::fflush(stderr);
    }

} // namespace intern

} // namespace p2ren

// https://github.com/scottt/debugbreak/blob/master/debugbreak.h
#ifndef NDEBUG
#    if defined(_MSC_VER)
#        define P2REN_DEBUG_BREAK __debugbreak()
#    else
#        define P2REN_DEBUG_BREAK __builtin_trap()
#    endif
#else
// Dont know if this works only  compiling on windows using MSVC and LLVM's Clang
#    define P2REN_DEBUG_BREAK std::DEBUG_BREAK(-1)
#endif

#ifndef NDEBUG
#    define P2REN_ASSERT(expression_, ...)                                                         \
        do                                                                                         \
        {                                                                                          \
            if (!(expression_))                                                                    \
            {                                                                                      \
                p2ren::intern::Error_OnAssert(                                                     \
                    __FILE__, __FUNCTION__, __LINE__, #expression_ __VA_OPT__(, ) __VA_ARGS__);    \
                P2REN_DEBUG_BREAK;                                                                 \
                std::exit(-1);                                                                     \
            }                                                                                      \
        }                                                                                          \
        while (false)

#    define P2REN_ASSERT_STRING_VIEW_NULL_TERMINATED(str_)                                         \
        P2REN_ASSERT(                                                                              \
            str_.data()[str_.size()] == '\0', "name parameter '{}' must be null terminated", str_)
#else
#    define P2REN_ASSERT(expression_, ...)
#    define P2REN_ASSERT_STRING_VIEW_NULL_TERMINATED(str_)
#endif

#define P2REN_FATAL(...)                                                                           \
    do                                                                                             \
    {                                                                                              \
        p2ren::intern::Error_OnLog(                                                                \
            p2ren::ErrorSeverity::High, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);            \
        P2REN_DEBUG_BREAK;                                                                         \
        std::exit(-1);                                                                             \
    }                                                                                              \
    while (false)

#define P2REN_INFO(...)                                                                            \
    p2ren::intern::Error_OnLog(                                                                    \
        p2ren::ErrorSeverity::Low, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define P2REN_WARN(...)                                                                            \
    p2ren::intern::Error_OnLog(                                                                    \
        p2ren::ErrorSeverity::Medium, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define P2REN_ERROR(...)                                                                           \
    p2ren::intern::Error_OnLog(                                                                    \
        p2ren::ErrorSeverity::High, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
