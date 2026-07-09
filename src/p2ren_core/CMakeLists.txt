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
    error.h
PRIVATE
    intern/application.cpp
    intern/error.cpp
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
PRIVATE
    SDL3::SDL3
)

