#include "glfwd_renderer/rhi/texture.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <glad/gl.h>
#include <glm/ext/vector_int2.hpp>

#include <array>

#include "glfwd_core/utility/error.h"
#include "glfwd_renderer/draw_modes.h"
#include "glfwd_renderer/rhi/context.h"

namespace glfwd {

Texture::Texture(const TextureCreateInfo info)
    : m_Type(info.Type),
      m_Width(info.Width),
      m_Height(info.Height),
      m_Depth(info.Depth)
{
    LoadOpenGLTexture(nullptr, info);
}

Texture::Texture(const uint8_t* buffer, size_t buffer_size, const TextureCreateInfo info)
    : m_Type(info.Type)
{
    if (buffer != nullptr && buffer_size > 0)
    {
        SDL_IOStream* stream = SDL_IOFromConstMem(buffer, buffer_size);
        if (!stream)
        {
            GLFWD_ERROR("Failed to load embedded {} texture buffer as a stream: {}",
                        TEXTURE_TYPE_NAMES[(int)info.Type],
                        SDL_GetError());
            return;
        }

        SDL_Surface* image_surface = IMG_Load_IO(stream, 1);
        if (!image_surface)
        {
            GLFWD_ERROR("Failed to load embedded {} texture buffer: {}",
                        TEXTURE_TYPE_NAMES[(int)info.Type],
                        SDL_GetError());
            SDL_CloseIO(stream);
        }
        SDL_CloseIO(stream);

        if (SetSizeFromSurface(image_surface, info))
            LoadOpenGLTexture(image_surface, info);
        else
            GLFWD_ERROR("Failed to load embedded {} texture", TEXTURE_TYPE_NAMES[(int)info.Type]);
        SDL_DestroySurface(image_surface);
    }
}

Texture::Texture(std::string_view path, const TextureCreateInfo info)
    : m_Type(info.Type)
{
    GLFWD_ASSERT_STRING_VIEW_NULL_TERMINATED(path);

    SDL_Surface* image_surface = IMG_Load(path.data());
    if (!image_surface)
    {
        GLFWD_ERROR("Failed to load surface at path {}: {}", path, SDL_GetError());
        return;
    }

    if (SetSizeFromSurface(image_surface, info))
    {
        LoadOpenGLTexture(image_surface, info);
    }
    else
    {
        GLFWD_ERROR(
            "Failed to load {} texture from path {}", TEXTURE_TYPE_NAMES[(int)info.Type], path);
    }
    SDL_DestroySurface(image_surface);
}

Texture::Texture(SDL_Surface* surface, const TextureCreateInfo info)
    : m_Type(info.Type)
{
    if (!surface)
    {
        if (info.Width < 0 || info.Height < 0)
        {
            GLFWD_ERROR(
                "Cannot create {} without a surface provided or at least a width and height",
                TEXTURE_TYPE_NAMES[(int)info.Type]);
            return;
        }

        m_Width  = info.Width;
        m_Height = info.Height;
        m_Depth  = info.Depth;
        LoadOpenGLTexture(nullptr, info);
    }
}

Texture::~Texture()
{
    if (m_ID != 0)
    {
        glDeleteTextures(1, &m_ID);
        m_ID = 0;
    }
}

Texture::Texture(Texture&& other)
    : m_ID(other.m_ID),
      m_Type(other.m_Type),
      m_Width(other.m_Width),
      m_Height(other.m_Height)
{
    other.m_ID     = 0;
    other.m_Width  = 0;
    other.m_Height = 0;
}

Texture& Texture::operator=(Texture&& other)
{
    if (this != &other)
    {
        if (m_ID != 0)
            glDeleteTextures(1, &m_ID);

        m_ID     = other.m_ID;
        m_Type   = other.m_Type;
        m_Width  = other.m_Width;
        m_Height = other.m_Height;

        other.m_ID = 0;
    }
    return *this;
}

void Texture::Bind() const
{
    glBindTexture(OpenGLContext::ConvertTextureTypeToOpenGL(m_Type), m_ID);
}

bool Texture::IsValid() const
{
    return m_ID != 0;
}

void Texture::LoadFromSurface(SDL_Surface* image_surface, const TextureCreateInfo& info)
{
    SDL_PixelFormat pixel_format;
    switch (info.Format)
    {
    case TextureFormat::RGB:   pixel_format = SDL_PIXELFORMAT_XRGB32; break;
    case TextureFormat::RGB8:  pixel_format = SDL_PIXELFORMAT_XRGB8888; break;
    case TextureFormat::RGBA:  pixel_format = SDL_PIXELFORMAT_RGBA32; break;
    case TextureFormat::RGBA8: pixel_format = SDL_PIXELFORMAT_RGBA8888; break;
    case TextureFormat::Depth24:
    case TextureFormat::Depth32f:
    case TextureFormat::Depth24Stencil8:
        GLFWD_ERROR("Do not provide image data when setting the format to Depth24. Will still "
                    "create texture but ignore image data.");
        LoadOpenGLTexture(nullptr, info);
        return;
    }

    if (info.FlipVertically)
        SDL_FlipSurface(image_surface, SDL_FLIP_VERTICAL);
    if (info.FlipHorizontally)
        SDL_FlipSurface(image_surface, SDL_FLIP_HORIZONTAL);

    if (pixel_format != image_surface->format)
    {
        SDL_Surface* surface = SDL_ConvertSurface(image_surface, pixel_format);
        if (!surface)
        {
            GLFWD_ERROR("Failed to convert image texture to desired format: {}", SDL_GetError());
            return;
        }
        LoadOpenGLTexture(surface, info);
        SDL_DestroySurface(surface);
    }
    else
        LoadOpenGLTexture(image_surface, info);
}

bool Texture::LoadOpenGLTexture(SDL_Surface* surface, const TextureCreateInfo& info)
{
    glGenTextures(1, &m_ID);

    uint32_t base_format;
    uint32_t output_format;
    OpenGLContext::ConvertTextureFormatToOpenGL(info.Format, base_format, output_format);

    // If multi-sample is enabled
    if (info.Sample != 1)
    {
        int32_t sample_count = info.Sample == 0 ? OpenGLContext::GetMaxMSAASamples() : info.Sample;

        if (info.Type != TextureType::Tex2D)
            GLFWD_WARN("Texture type not set to TextureType::Tex2D when creating a multi-sampled "
                       "texture. Will overwrite and set to Tex2D with multi-sampling enabled");

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID);
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE, sample_count, base_format, m_Width, m_Height, GL_TRUE);

        return false;
    }

    uint32_t type = OpenGLContext::ConvertTextureTypeToOpenGL(info.Type);
    glBindTexture(type, m_ID);

    switch (info.Type)
    {
    case TextureType::Tex2D:
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     base_format,
                     m_Width,
                     m_Height,
                     0,
                     output_format,
                     GL_UNSIGNED_BYTE,
                     surface ? surface->pixels : nullptr);
        break;
    case TextureType::Tex3D:
        glTexImage3D(GL_TEXTURE_2D,
                     0,
                     base_format,
                     m_Width,
                     m_Height,
                     m_Depth,
                     0,
                     output_format,
                     GL_UNSIGNED_BYTE,
                     surface ? surface->pixels : nullptr);
        break;
    case TextureType::CubeMap:
        if (!LoadOpenGLCubeMapFromSurface(surface, info, base_format, output_format))
        {
            glDeleteTextures(1, &m_ID);
            m_ID = 0;
            return false;
        }
        break;
    }

    // Handle texture wrapping
    int32_t wrapping = OpenGLContext::ConvertTextureWrapToOpenGL(info.Wrap);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapping);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapping);
    if (info.Type == TextureType::Tex3D)
        glTexParameteri(type, GL_TEXTURE_WRAP_R, wrapping);

    int32_t min_filter, mag_filter;
    OpenGLContext::ConvertTextureFiltersToOpenGL(
        info.MinFilter, info.MagFilter, info.Mipmap, min_filter, mag_filter);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag_filter);

    if (info.Anisotropy != 1)
    {
        glTexParameterf(type,
                        GL_TEXTURE_MAX_ANISOTROPY,
                        static_cast<float>(info.Anisotropy == 0 ? OpenGLContext::GetMaxAnisotropy()
                                                                : info.Anisotropy));
    }

    // Enable mipmapping
    if (info.Mipmap != MipmapMode::None)
        glGenerateMipmap(type);

    GLFWD_INFO("Successfully initialized {} ({}, {})",
               TEXTURE_TYPE_NAMES[(int)info.Type],
               m_Width,
               m_Height);
    return true;
}

bool Texture::LoadOpenGLCubeMapFromSurface(SDL_Surface* surface, const TextureCreateInfo& info,
                                           int32_t base_format, int32_t output_format)
{
    if (!surface)
    {
        GLFWD_ERROR("Cube map texture must provide a surface to create");
        return false;
    }
    if (info.Format > TextureFormat::RGBA)
    {
        GLFWD_ERROR("Invalid texture format for creating a cube map. Must be a color format not a "
                    "depth/stencil");
        return false;
    }

    std::array<glm::ivec2, 6> face_offsets = {
        glm::ivec2{2, 1}, // Right    (Positive X)
        glm::ivec2{0, 1}, // Left     (Negative X)
        glm::ivec2{1, 0}, // Top      (Positive Y)
        glm::ivec2{1, 2}, // Bottom   (Negative Y)
        glm::ivec2{1, 1}, // Front    (Positive Z)
        glm::ivec2{3, 1}, // Back     (Negative Z)
    };

    // Set row element length
    int32_t color_element_count = info.Format <= TextureFormat::RGB8 ? 3 : 4;
    glPixelStorei(GL_UNPACK_ROW_LENGTH, surface->pitch / color_element_count);

    // Iterate over all textures within the image, offsetting by there position * face_size
    for (int32_t i = 0; i < face_offsets.size(); i++)
    {
        const glm::ivec2 offset = face_offsets[i];
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, offset.x * m_Width); // Skip column chunks
        glPixelStorei(GL_UNPACK_SKIP_ROWS, offset.y * m_Height);  // Skip row chunks
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,
                     base_format,
                     m_Width,
                     m_Height,
                     0,
                     output_format,
                     GL_UNSIGNED_BYTE,
                     surface->pixels);
    }

    // Reset state to not fuck up everything else
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    return true;
}

bool Texture::SetSizeFromSurface(SDL_Surface* surface, const TextureCreateInfo& info)
{
    if (surface)
    {
        m_Width  = surface->w;
        m_Height = surface->h;
        if (info.Type == TextureType::CubeMap)
        {
            m_Width  /= 4;
            m_Height /= 3;
        }
        return true;
    }
    GLFWD_ERROR("Surface is null, cannot determine texture width and height");
    return false;
}

} // namespace glfwd
