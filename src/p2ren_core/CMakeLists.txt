# Core
# ==============================================================================
add_library(p2ren_core STATIC)
target_include_directories(p2ren_core
PUBLIC
    ${ROOT_SRC_DIR}
)

target_sources(p2ren_core
PUBLIC
    application.h
    application_descriptor.h
    error.h
    hash.h
    plugin.h
    type_info.h
    window.h
PRIVATE
    intern/application.cpp
    intern/error.cpp
    intern/window.cpp
)

# Options
# ==============================================================================
if (MSVC)
    target_compile_options(p2ren_core
    PUBLIC
        /Zc:preprocessor # Needed for __VA_OPT__() macro
    )
endif()

# Dependencies
# ==============================================================================
target_link_libraries(p2ren_core
PUBLIC
    fmt::fmt
    glm::glm
    unordered_dense::unordered_dense
PRIVATE
    SDL3::SDL3
)

