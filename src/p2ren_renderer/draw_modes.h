#pragma once

#include <array>
#include <string_view>

namespace p2ren {

enum class PolygonMode
{
    FillFrontAndBack,
    FillFront,
    FillBack,
    Lines,
};

enum class PrimitiveMode
{
    Points,        // Individual vertex point
    Lines,         // Individual lines, disconnected
    LineStrip,     // Connected lines, curved lines, Bezier curves
    LineLoop,      // Connected lines which wraps back to first vertex
    Triangles,     // Individual objects, models
    TriangleStrip, // Ribbons, generating roads
    TriangleFan,   // Circular objects
    Patches,       // Let the gpu subdivide geometry, tessellation
};

enum class TextureType
{
    Tex2D,
    Tex3D,
    CubeMap,
};
constexpr std::array<std::string_view, 3> s_TextureTypeNames = {
    "Texture 2D",
    "Texture 3D",
    "Cube Map",
};

enum class TextureWrap
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
};

enum class TextureFormat
{
    RGB,
    RGB8,
    RGBA,
    RGBA8,
    Depth24,
    Depth24Stencil8,
    Depth32f,
};

enum class TextureFilter
{
    Nearest,
    Linear,
};

enum class MipmapMode
{
    None,
    Nearest,
    Linear,
};

} // namespace p2ren
