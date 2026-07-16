#pragma once

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

} // namespace p2ren
