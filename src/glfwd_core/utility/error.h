#pragma once

#include <fmt/core.h>
#include <fmt/std.h> // IWYU pragma: export

#include <cstdio>
#include <string_view>

namespace glfwd {

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
    void Error_PrintAssertHeader(ErrorSeverity severity, std::string_view file,
                                 std::string_view function, int line, std::string_view expression);

    /// Prints error message
    void Error_PrintHeader(ErrorSeverity severity, std::string_view file, std::string_view function,
                           int line);

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
        Error_PrintAssertHeader(ErrorSeverity::Fatal, file, function, line, expression);

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

        Error_PrintHeader(severity, file, function, line);

        fmt::println(stderr, format, std::forward<Args>(args)...);
        std::fflush(stderr);
    }

} // namespace intern

} // namespace glfwd

// https://github.com/scottt/debugbreak/blob/master/debugbreak.h
#ifndef NDEBUG
#    if defined(_MSC_VER)
#        define glfwd_DEBUG_BREAK __debugbreak()
#    else
#        define glfwd_DEBUG_BREAK __builtin_trap()
#    endif
#else
// Dont know if this works only  compiling on windows using MSVC and LLVM's Clang
#    define glfwd_DEBUG_BREAK std::DEBUG_BREAK(-1)
#endif

#ifndef NDEBUG
#    define GLFWD_ASSERT(expression_, ...)                                                         \
        do                                                                                         \
        {                                                                                          \
            if (!(expression_))                                                                    \
            {                                                                                      \
                glfwd::intern::Error_OnAssert(                                                     \
                    __FILE__, __FUNCTION__, __LINE__, #expression_ __VA_OPT__(, ) __VA_ARGS__);    \
                glfwd_DEBUG_BREAK;                                                                 \
                std::exit(-1);                                                                     \
            }                                                                                      \
        }                                                                                          \
        while (false)

#    define GLFWD_ASSERT_STRING_VIEW_NULL_TERMINATED(str_)                                         \
        GLFWD_ASSERT(                                                                              \
            str_.data()[str_.size()] == '\0', "name parameter '{}' must be null terminated", str_)
#else
#    define GLFWD_ASSERT(expression_, ...)
#    define GLFWD_ASSERT_STRING_VIEW_NULL_TERMINATED(str_)
#endif

#define GLFWD_FATAL(...)                                                                           \
    do                                                                                             \
    {                                                                                              \
        glfwd::intern::Error_OnLog(                                                                \
            glfwd::ErrorSeverity::Fatal, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);           \
        glfwd_DEBUG_BREAK;                                                                         \
        std::exit(-1);                                                                             \
    }                                                                                              \
    while (false)

#define GLFWD_INFO(...)                                                                            \
    glfwd::intern::Error_OnLog(                                                                    \
        glfwd::ErrorSeverity::Low, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define GLFWD_WARN(...)                                                                            \
    glfwd::intern::Error_OnLog(                                                                    \
        glfwd::ErrorSeverity::Medium, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define GLFWD_ERROR(...)                                                                           \
    glfwd::intern::Error_OnLog(                                                                    \
        glfwd::ErrorSeverity::High, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
