cmake_minimum_required(VERSION 3.30)
project(p2ren
    LANGUAGES CXX C
)

cmake_policy(SET CMP0003 NEW)
# Syntax problems are errors
cmake_policy(SET CMP0010 NEW)

# Removes unnecessary cmake files when generating MSVC project files
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# ==============================================================================
# Build settings
# ==============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(BINARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}/Archive")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")

set(ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Root Directory")

# ==============================================================================
# Library
# ==============================================================================

add_library(p2ren STATIC)
add_subdirectory(src/p2ren_core)
add_subdirectory(src/p2ren_render_hardware)
add_subdirectory(src/p2ren_renderer)
add_subdirectory(src/p2ren_world)

target_include_directories(p2ren
PUBLIC
    ${CMAKE_cURRENT_SOURCE_DIR}/src
)

if (MSVC)
    target_compile_options(p2ren
    PUBLIC
        /Zc:preprocessor # Needed for __VA_OPT__() macro
    )
endif()

add_subdirectory(src/p2ren_main)
