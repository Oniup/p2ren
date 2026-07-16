# Engine
# ==============================================================================
add_library(p2ren_engine STATIC)
target_include_directories(p2ren_engine
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_sources(p2ren_engine
PUBLIC
    ./application.h
    ./create_info.h
PRIVATE
    ./application.cpp
)

# Dependencies
# ==============================================================================
target_link_libraries(p2ren_engine
PUBLIC
    p2ren::core
    p2ren::renderer
    p2ren::world

PRIVATE
    SDL3::SDL3 # TODO: Remove later
    glad       # TODO: Remove later
)

# Alias for external linking
# ==============================================================================
add_library(p2ren::engine ALIAS p2ren_engine)

