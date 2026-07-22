#pragma once

#include <glm/ext/vector_float4.hpp>

#include <cstdint>
#include <string_view>

#include "p2ren_renderer/draw_modes.h"

struct SDL_Surface;

namespace p2ren {

struct TextureCreateInfo
{
    size_t Width  = 0; // Must define if creating an empty texture
    size_t Height = 0; // Must define if creating an empty texture
    size_t Depth  = 0;

    TextureType   Type   = TextureType::Tex2D;
    TextureFormat Format = TextureFormat::RGBA;
    TextureWrap   Wrap   = TextureWrap::Repeat;

    TextureFilter MinFilter  = TextureFilter::Nearest;
    TextureFilter MagFilter  = TextureFilter::Nearest;
    MipmapMode    Mipmap     = MipmapMode::Linear;
    uint32_t      Anisotropy = 1; // 1 => Disables, 0 => Max

    glm::vec4 BorderColor         = glm::vec4(0.0f);
    bool      ApplySRGBCorrection = true;
    bool      FlipVertically      = true;
    bool      FlipHorizontally    = false;
    uint32_t  Sample              = 1; // 1 => Disables, 0 => Max
};

class Texture
{
public:
    Texture() = default;
    Texture(const TextureCreateInfo info);
    Texture(const uint8_t* buffer, size_t buffer_size, const TextureCreateInfo info);
    Texture(std::string_view path, const TextureCreateInfo info);
    Texture(SDL_Surface* surface, const TextureCreateInfo info);
    ~Texture();

    Texture(Texture&& other);
    Texture& operator=(Texture&& other);

    Texture(const Texture& other)            = delete;
    Texture& operator=(const Texture& other) = delete;

    void Bind() const;
    bool IsValid() const;

    TextureType GetType() const { return m_Type; }
    uint32_t    GetWidth() const { return m_Width; }
    uint32_t    GetHeight() const { return m_Height; }

private:
    void LoadFromSurface(SDL_Surface* image_surface, const TextureCreateInfo& info);
    bool LoadOpenGLTexture(SDL_Surface* surface, const TextureCreateInfo& info);
    bool LoadOpenGLCubeMapFromSurface(SDL_Surface* surface, const TextureCreateInfo& info,
                                      int32_t base_format, int32_t output_format);

    bool SetSizeFromSurface(SDL_Surface* surface, const TextureCreateInfo& info);

    uint32_t    m_ID = 0;
    TextureType m_Type;
    uint32_t    m_Width  = 0;
    uint32_t    m_Height = 0;
    uint32_t    m_Depth  = 0;
};

} // namespace p2ren
