#pragma once

#include <cstdint>

#include "glfwd_renderer/draw_modes.h"

typedef struct SDL_GLContextState* SDL_GLContext;

namespace glfwd {

class Window;

class OpenGLContext
{
public:
    // Draw mode
    static int32_t ConvertPolygonModeToOpenGL(PolygonMode mode);
    static int32_t ConvertPrimitiveModeToOpenGL(PrimitiveMode mode);

    // Texture
    static uint32_t ConvertTextureTypeToOpenGL(TextureType type);
    static int32_t  ConvertTextureWrapToOpenGL(TextureWrap wrap);
    static void     ConvertTextureFormatToOpenGL(TextureFormat format, uint32_t& base_format,
                                                 uint32_t& output_format);
    static void ConvertTextureFiltersToOpenGL(TextureFilter min_filter, TextureFilter mag_filter,
                                              MipmapMode mipmap, int32_t& min_out,
                                              int32_t& mag_out);

    static int32_t GetMaxMSAASamples();
    static int32_t GetMaxAnisotropy();

    OpenGLContext(bool enable_hardware_debug_callback);
    ~OpenGLContext();

    void InitializeBackend(Window* window);
    void Terminate();

    SDL_GLContext       GetInternalContext() { return m_InternalContext; }
    const SDL_GLContext GetInternalContext() const { return m_InternalContext; }

private:
    static OpenGLContext* s_Instance;

    SDL_GLContext m_InternalContext = nullptr;
    int32_t       m_MaxMSAASamples;
    int32_t       m_MaxAnisotropy;
};

} // namespace glfwd
