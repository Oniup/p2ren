# Renderer and OpenGL Abstraction Layer
# ==============================================================================
add_library(glfwd_renderer STATIC)
target_include_directories(glfwd_renderer
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_sources(glfwd_renderer
PUBLIC
    ./resources/camera.h
    ./resources/material.h
    ./resources/model.h
    ./rhi/buffer.h
    ./rhi/context.h
    ./rhi/shader.h
    ./rhi/texture.h
    ./create_info.h
    ./render_queue.h
    ./draw_modes.h
    ./forward_renderer.h
PRIVATE
    ./resources/camera.cpp
    ./resources/material.cpp
    ./resources/model.cpp
    ./rhi/buffer.cpp
    ./rhi/context.cpp
    ./rhi/shader.cpp
    ./rhi/texture.cpp
    ./forward_renderer.cpp
)

target_compile_definitions(glfwd_renderer
PRIVATE
    glfwd_SHADER_HOT_RELOAD=$<BOOL:${glfwd_SHADER_HOT_RELOAD}>
)

# Dependencies
# ==============================================================================
target_link_libraries(glfwd_renderer
PUBLIC
    glfwd::core
    glm::glm

PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
)

# Alias for external linking
# ==============================================================================
add_library(glfwd::renderer ALIAS glfwd_renderer)
