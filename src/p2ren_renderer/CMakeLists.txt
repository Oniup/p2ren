# Renderer and OpenGL Abstraction Layer
# ==============================================================================
add_library(p2ren_renderer STATIC)
target_include_directories(p2ren_renderer
PUBLIC
    ${ROOT_SRC_DIR}
)

target_sources(p2ren_renderer
PUBLIC
    ./resources/material.h
    ./resources/mesh.h
    ./resources/model.h
    ./rhi/buffer.h
    ./rhi/shader.h
    ./rhi/texture.h
    ./renderer.h
PRIVATE
    ./resources/material.cpp
    ./resources/mesh.cpp
    ./resources/model.cpp
    ./rhi/buffer.cpp
    ./rhi/shader.cpp
    ./rhi/texture.cpp
    ./renderer.cpp

)

target_compile_definitions(p2ren_renderer
PRIVATE
    P2REN_SHADER_HOT_RELOAD=$<BOOL:${P2REN_SHADER_HOT_RELOAD}>
)

# Dependencies
# ==============================================================================
target_link_libraries(p2ren_renderer
PUBLIC
    p2ren_core
    glm::glm

PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
)


