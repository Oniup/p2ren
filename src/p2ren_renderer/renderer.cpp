#include "p2ren_renderer/renderer.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>

#include "p2ren_core/application_descriptor.h"
#include "p2ren_core/utility/error.h"
#include "p2ren_core/window.h"

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

Renderer::~Renderer()
{
    Terminate();
}

void Renderer::Initialize(const RendererDescriptor& descriptor)
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
    if (descriptor.EnableOpenGLDebugCallback)          // Enable Debug error callback
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
}

void Renderer::InitializeBackend(const Window& window, const RendererDescriptor& descriptor)
{
    m_InternalContext = SDL_GL_CreateContext(window.GetInternalContext());
    if (!m_InternalContext)
        P2REN_FATAL("Failed to initialize OpenGL context: {}", SDL_GetError());

    int glad_version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    if (glad_version == 0)
    {
        Terminate();
        P2REN_FATAL("Failed to initialize OpenGL pointer functions through glad loader");
    }

    P2REN_INFO("Initialized OpenGL {}.{}",
               GLAD_VERSION_MAJOR(glad_version),
               GLAD_VERSION_MINOR(glad_version));

    int32_t width, height;
    window.GetSize(&width, &height);
    glViewport(0, 0, width, height);

    // Debug callback setup
    if (descriptor.EnableOpenGLDebugCallback)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutputCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}

void Renderer::Terminate()
{
    if (m_InternalContext)
    {
        SDL_GL_DestroyContext(m_InternalContext);
        SDL_Quit();
    }
}

void Renderer::SwapBuffers(const Window& window)
{
    SDL_GL_SwapWindow(window.GetInternalContext());
}

} // namespace p2ren
