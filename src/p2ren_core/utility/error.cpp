#include "p2ren_core/utility/error.h"

#include <fmt/color.h>
#include <fmt/core.h>

namespace p2ren {

static ErrorSeverity s_FilterLower = ErrorSeverity::Low;

void Error_SetFilterLower(ErrorSeverity severity)
{
    s_FilterLower = severity;
}

namespace intern {

    fmt::text_style GetSeverityStyle(ErrorSeverity severity)
    {
        switch (severity)
        {
        case ErrorSeverity::Low: return fmt::fg(fmt::color::light_sky_blue);
        case ErrorSeverity::Medium:
            return fmt::fg(fmt::color::light_golden_rod_yellow) | fmt::emphasis::bold;
        case ErrorSeverity::High: return fmt::fg(fmt::color::dark_red);
        case p2ren::ErrorSeverity::Fatal:
            return fmt::fg(fmt::color::dark_red) | fmt::emphasis::bold;
        default: fmt::fg(fmt::color::white);
        }
    }

    std::string_view GetSeverityName(ErrorSeverity severity)
    {
        switch (severity)
        {
        case ErrorSeverity::Low:    return "LOW";
        case ErrorSeverity::Medium: return "MEDIUM";
        case ErrorSeverity::High:   return "HIGH";
        case ErrorSeverity::Fatal:  return "FATAL";
        default:                    return "UNKNOWN";
        }
    }

    bool Error_ShouldFilter(ErrorSeverity severity)
    {
        return severity < s_FilterLower;
    }

    void Error_PrintErrorHeaderMessage(ErrorSeverity severity, std::string_view file,
                                       std::string_view function, int line,
                                       std::string_view expression)
    {
        fmt::text_style  severity_style = GetSeverityStyle(severity);
        std::string_view severity_name  = GetSeverityName(severity);
        fmt::print(stderr,
                   severity_style,
                   "[{}]: in '{}' at {}:{} --> '{}'\n",
                   severity_name,
                   file,
                   function,
                   line,
                   expression);
    }

    void Error_PrintAssertErrorMessage(ErrorSeverity severity, std::string_view file,
                                       std::string_view function, int line)
    {
        fmt::text_style  severity_style = GetSeverityStyle(severity);
        std::string_view severity_name  = GetSeverityName(severity);
        fmt::print(stderr,
                   severity_style,
                   "[{}]: in '{}' at {}:{}\n",
                   severity_name,
                   file,
                   function,
                   line);
    }

    void Error_OnAssert(std::string_view file, std::string_view function, int line,
                        std::string_view expression)
    {
        Error_PrintErrorHeaderMessage(ErrorSeverity::Fatal, file, function, line, expression);
    }

} // namespace intern

} // namespace p2ren
