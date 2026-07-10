# Render Hardware Abstraction (OpenGL)
# ==============================================================================
add_library(p2ren_rhi STATIC)
target_include_directories(p2ren_core
PUBLIC
    ${ROOT_SRC_DIR}
)

target_sources(p2ren_rhi
PUBLIC
    context.h
PRIVATE
    intern/context.cpp
)

# Dependencies
# ==============================================================================
target_link_libraries(p2ren_rhi
PUBLIC
    p2ren_core
    glm::glm

PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
)

