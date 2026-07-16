#include "p2ren_renderer/rhi/context.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "p2ren_core/utility/error.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/draw_modes.h"

namespace p2ren {

#define EFMT "OpenGL ID: {}, Source: {}, Type: {}: {}"
void GLAPIENTRY glDebugOutputCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
                                      GLsizei length, const char* message, const void* userParam)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::string_view source_str;
    std::string_view type_str;
    std::string_view severity_str;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             source_str = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     source_str = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           source_str = "Other"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               type_str = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         type_str = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         type_str = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              type_str = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          type_str = "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           type_str = "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               type_str = "Other"; break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         P2REN_FATAL(EFMT, id, source_str, type_str, message); break;
    case GL_DEBUG_SEVERITY_MEDIUM:       P2REN_ERROR(EFMT, id, source_str, type_str, message); break;
    case GL_DEBUG_SEVERITY_LOW:          P2REN_WARN(EFMT, id, source_str, type_str, message); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: P2REN_INFO(EFMT, id, source_str, type_str, message); break;
    }
}

int32_t RHIContext::ConvertPolygonModeToRHI(PolygonMode mode)
{
    switch (mode)
    {
    case PolygonMode::FillFrontAndBack: return GL_FRONT_AND_BACK;
    case PolygonMode::FillFront:        return GL_FRONT;
    case PolygonMode::FillBack:         return GL_BACK;
    case PolygonMode::Lines:            return GL_LINE;
    }
}

int32_t RHIContext::ConvertPrimitiveModeToRHI(PrimitiveMode mode)
{
    switch (mode)
    {
    case PrimitiveMode::Points:        return GL_POINTS;
    case PrimitiveMode::Lines:         return GL_LINE;
    case PrimitiveMode::LineStrip:     return GL_LINE_STRIP;
    case PrimitiveMode::LineLoop:      return GL_LINE_LOOP;
    case PrimitiveMode::Triangles:     return GL_TRIANGLES;
    case PrimitiveMode::TriangleStrip: return GL_TRIANGLE_STRIP;
    case PrimitiveMode::TriangleFan:   return GL_TRIANGLE_FAN;
    case PrimitiveMode::Patches:       return GL_PATCHES;
    }
}

RHIContext::RHIContext(bool enable_hardware_debug_callback)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        P2REN_FATAL("Failed to initialize SDL: {}", SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);       // Required to prevent screen tearing
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);         // Screen framebuffer doesn't require
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);       // Screen framebuffer doesn't require
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Disable software rendering

    if (enable_hardware_debug_callback)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
}

RHIContext::~RHIContext()
{
    Terminate();
}

void RHIContext::InitializeBackend(Window* window)
{
    m_InternalContext = SDL_GL_CreateContext(window->GetInternalContext());
    if (!m_InternalContext)
        P2REN_FATAL("Failed to initialize OpenGL context: {}", SDL_GetError());

    // Initialize glad
    int glad_version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (glad_version == 0)
    {
        Terminate();
        P2REN_FATAL("Failed to initialize OpenGL pointer functions through glad loader");
    }
    P2REN_INFO("Initialized OpenGL {}.{}",
               GLAD_VERSION_MAJOR(glad_version),
               GLAD_VERSION_MINOR(glad_version));

    // Debug callback setup
    int32_t flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutputCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

void RHIContext::Terminate()
{
    if (m_InternalContext)
    {
        SDL_GL_DestroyContext(m_InternalContext);
        SDL_Quit();

        m_InternalContext = nullptr;
    }
}

} // namespace p2ren
