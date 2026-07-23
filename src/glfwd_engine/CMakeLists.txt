# Engine
# ==============================================================================
add_library(glfwd_engine STATIC)
target_include_directories(glfwd_engine
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_sources(glfwd_engine
PUBLIC
    ./application.h
    ./create_info.h
PRIVATE
    ./application.cpp
)

# Dependencies
# ==============================================================================
target_link_libraries(glfwd_engine
PUBLIC
    glfwd::core
    glfwd::renderer

    SDL3::SDL3 # TODO: Remove later
    glad       # TODO: Remove Later
)

# Alias for external linking
# ==============================================================================
add_library(glfwd::engine ALIAS glfwd_engine)
