#pragma once

#include "glfwd_core/resource_handle.h"
#include "glfwd_core/transform.h"
#include "glfwd_engine/application.h"
#include "glfwd_renderer/render_queue.h"
#include "glfwd_renderer/resources/model.h"
#include "glfwd_renderer/rhi/shader.h"

namespace glfwd_example {

class Game : public glfwd::Application
{
protected:
    void OnInitialize() override;
    void OnUpdate(const glfwd::Timestep& timestep) override;
    void OnLateUpdate(const glfwd::Timestep& timestep) override;
    void SubmitToRenderQueue(glfwd::RenderQueue* render_queue) override;

private:
    glfwd::Transform                     m_Transform;
    glfwd::Mesh                          m_Mesh;
    glfwd::ResourceHandle<glfwd::Shader> m_ShaderHandle;
};

} // namespace glfwd_example
