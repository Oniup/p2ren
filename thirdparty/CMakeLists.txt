# Platform
# ==============================================================================
add_subdirectory(SDL)       # Window, IO, Audio, Render Hardware backend context
add_subdirectory(SDL_image) # Loading and parsing images

# Utilities
# ==============================================================================
add_subdirectory(fmt)               # String formatting library
add_subdirectory(glm)               # Math Library
add_subdirectory(unordered_dense)   # High performance flat unordered map

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
