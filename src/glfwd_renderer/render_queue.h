#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

#include "glfwd_core/resource_handle.h"
#include "glfwd_renderer/resources/model.h"

namespace glfwd {

class RenderQueue
{
    struct DrawCommand
    {
        glm::mat4             ModelMatrix   = glm::mat4(1.0f);
        ResourceHandle<Model> ModelHandle   = ResourceHandle<Model>{};
        glm::vec3             ColorOverlay  = glm::vec3(1.0f);
        bool                  IsTransparent = false;
        bool                  EnableShadows = true;
    };

public:
    void Submit(const glm::mat4& transform_matrix, const ResourceHandle<Model>& model);
    void Clear();

    void Sort();

private:
    std::vector<DrawCommand> m_Commands;
};

} // namespace glfwd
