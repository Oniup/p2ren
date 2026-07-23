#include "glfwd_renderer/resources/camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace glfwd {

RenderCamera CameraBase::GetRenderCamera(int32_t viewport_width, int32_t viewport_height) const
{
    return RenderCamera{
        .Position         = Position,
        .ClearColor       = ClearColor,
        .ViewMatrix       = CreateViewMatrix(),
        .ProjectionMatrix = Projection == ProjectionMode::Perspective
                                ? glm::perspective(glm::radians(FieldOfView),
                                                   static_cast<float>(viewport_width) /
                                                       static_cast<float>(viewport_height),
                                                   NearClippingPlane,
                                                   FarClippingPlane)
                                : glm::ortho(0.0f,
                                             static_cast<float>(viewport_width),
                                             0.0f,
                                             static_cast<float>(viewport_height)),
    };
}

glm::mat4 TargetCamera::CreateViewMatrix() const
{
    return glm::lookAt(Position, Target, Up);
}

glm::mat4 DirectionalCamera::CreateViewMatrix() const
{
    return glm::lookAt(Position, Position + Forward, Up);
}

} // namespace glfwd
