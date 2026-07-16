#pragma once

#include <string_view>
#include <vector>

#include "p2ren_renderer/resources/mesh.h"

namespace p2ren {

class Model
{
public:
    Model() = default;
    Model(std::string_view path);

private:
    std::vector<Mesh> m_Meshes;
};

} // namespace p2ren
