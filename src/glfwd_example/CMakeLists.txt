add_executable(glfwd_example
    ./game.h
    ./game.cpp
    ./main.cpp
)

target_link_libraries(glfwd_example
PRIVATE
    glfwd::engine
)
