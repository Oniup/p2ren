# Core
# ==============================================================================
add_library(glfwd_core STATIC)
target_include_directories(glfwd_core
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_sources(glfwd_core
PUBLIC
    ./utility/color.h
    ./utility/error.h
    ./utility/hash.h
    ./utility/type_info.h
    ./resource_handle.h
    ./resource_manager.h
    ./time.h
    ./transform.h
    ./window.h
PRIVATE
    ./utility/color.cpp
    ./utility/error.cpp
    ./time.cpp
    ./transform.cpp
    ./window.cpp
)

# Options
# ==============================================================================
if (MSVC)
    target_compile_options(glfwd_core
    PUBLIC
        /Zc:preprocessor # Needed for __VA_OPT__() macro
    )
endif()

# Dependencies
# ==============================================================================
target_link_libraries(glfwd_core
PUBLIC
    fmt::fmt
    glm::glm
    unordered_dense::unordered_dense
PRIVATE
    SDL3::SDL3
)

# Alias for external linking
# ==============================================================================
add_library(glfwd::core ALIAS glfwd_core)
