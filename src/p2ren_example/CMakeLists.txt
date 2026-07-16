add_executable(p2ren_example
    ./game.h
    ./game.cpp
    ./main.cpp
)

target_link_libraries(p2ren_example
PRIVATE
    p2ren::engine
)
