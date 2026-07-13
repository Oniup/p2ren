# Platform
# ==============================================================================
set(SDLIMAGE_AVIF OFF CACHE BOOL "Disable AVIF support" FORCE)

add_subdirectory(SDL)       # Window, IO, Audio, Render Hardware backend context
add_subdirectory(SDL_image) # Loading and parsing images

# Utilities
# ==============================================================================
add_subdirectory(fmt)               # String formatting library
add_subdirectory(unordered_dense)   # High performance flat unordered map

add_library(glm INTERFACE) # Math Library
target_include_directories(glm SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)
add_library(glm::glm ALIAS glm)

# Render Hardware
# ==============================================================================
# Setup Glad -- Loading OpenGL pointer function macros
add_library(glad
STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/gl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/gl.c
)

target_include_directories(glad
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)
