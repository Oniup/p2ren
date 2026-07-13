add_executable(p2ren_example
    ./program.h
    ./program.cpp
    ./main.cpp
)

target_link_libraries(p2ren_example
PRIVATE
    p2ren::p2ren
)
