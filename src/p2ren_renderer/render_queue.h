#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

#include "p2ren_core/resource_handle.h"
#include "p2ren_renderer/resources/model.h"

namespace p2ren {

class RenderQueue
{
    struct DrawCommand
    {
        glm::mat4             ModelMatrix;
        ResourceHandle<Model> ModelHandle;
        glm::vec3             ColorOverlay;
    };

public:
    void Submit(const glm::mat4& transform_matrix, const ResourceHandle<Model>& model);
    void Clear();

    void Sort();

private:
    std::vector<DrawCommand> m_Commands;
};

} // namespace p2ren
