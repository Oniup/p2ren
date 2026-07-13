# Core
# ==============================================================================
add_library(p2ren_core STATIC)
target_include_directories(p2ren_core
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_sources(p2ren_core
PUBLIC
    ./utility/color.h
    ./utility/error.h
    ./utility/hash.h
    ./utility/type_info.h
    ./application.h
    ./application_descriptor.h
    ./resource_handle.h
    ./resource_manager.h
    ./window.h
PRIVATE
    ./utility/color.cpp
    ./utility/error.cpp
    ./application.cpp
    ./window.cpp
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

# Alias for external linking
# ==============================================================================
add_library(p2ren::core ALIAS p2ren_core)