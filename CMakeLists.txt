cmake_minimum_required(VERSION 3.30)
project(p2ren
    LANGUAGES CXX C
)

cmake_policy(SET CMP0003 NEW)
# Syntax problems are errors
cmake_policy(SET CMP0010 NEW)

# Removes unnecessary cmake files when generating MSVC project files
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Build settings and options
# ==============================================================================

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

# Build Directory
set(BINARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}/Archive")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")

set(ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Root Directory")
set(ROOT_SRC_DIR "${ROOT_DIRECTORY}/src" CACHE INTERNAL "Root source directory")

# Options
option(BUILD_MAIN_EXAMPLE "Build example executable" ON)

# Library
# ==============================================================================

# Thirdparty
add_subdirectory(thirdparty SYSTEM)

# Modules
add_subdirectory(src/p2ren_core)
# add_subdirectory(src/p2ren_render_hardware)
# add_subdirectory(src/p2ren_renderer)
# add_subdirectory(src/p2ren_world)

# Interface
add_library(p2ren INTERFACE)
target_link_libraries(p2ren
INTERFACE
    p2ren_core
)
target_link_libraries(p2ren
INTERFACE
    ${ROOT_SRC_DIR}
)


if (${BUILD_MAIN_EXAMPLE})
    add_subdirectory(src/p2ren_example)
endif()
