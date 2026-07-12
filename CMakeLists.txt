cmake_minimum_required(VERSION 3.30)
project(p2ren
    LANGUAGES CXX C
)

# Removes unnecessary cmake files when generating MSVC project files
set(CMAKE_SUPPRESS_REGENERATION TRUE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_EXTENSIONS OFF) # For cross platform/compiler compatibility

# Build settings and options
# ==============================================================================
# Options
option(P2REN_BUILD_EXAMPLE "Build example executable" ON)
option(P2REN_SHADER_HOT_RELOAD "Allows for hot reloading shaders. Shaders will still their paths now" ON)

# Build directory setup for when building example is enabled
if (P2REN_BUILD_EXAMPLE)
    set(BINARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/$<CONFIG>")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}/Archive")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")
endif(0)

set(ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Root Directory")
set(ROOT_SRC_DIR "${ROOT_DIRECTORY}/src" CACHE INTERNAL "Root source directory")

# Library
# ==============================================================================
add_subdirectory(thirdparty SYSTEM) # Thirdparty

# Modules
add_subdirectory(src/p2ren_core)
add_subdirectory(src/p2ren_renderer)
# add_subdirectory(src/p2ren_world) # Not started yet

# P2Ren interface library
add_library(p2ren INTERFACE)
target_link_libraries(p2ren
INTERFACE
    p2ren_core
    p2ren_renderer
)
target_link_libraries(p2ren
INTERFACE
    $<BUILD_INTERFACE:${ROOT_SRC_DIR}>
)

if (${P2REN_BUILD_EXAMPLE})
    add_subdirectory(src/p2ren_example)
endif()
