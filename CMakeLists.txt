cmake_minimum_required(VERSION 3.30)
project(glfwd
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
option(glfwd_BUILD_EXAMPLE "Build example executable" ON)

# Build directory setup for when building example is enabled
if (glfwd_BUILD_EXAMPLE)
    set(BINARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/$<CONFIG>")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}/Archive")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}")
endif()

# Library
# ==============================================================================
add_subdirectory(thirdparty SYSTEM) # Thirdparty

# Modules
add_subdirectory(src/glfwd_core) # Load first
add_subdirectory(src/glfwd_renderer)
add_subdirectory(src/glfwd_engine) # Load last

if (${glfwd_BUILD_EXAMPLE})
    add_subdirectory(src/glfwd_example)
endif()
