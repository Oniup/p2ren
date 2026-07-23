#include "glfwd_engine/create_info.h"
#include "glfwd_example/game.h"

int main()
{
    glfwd_example::Game game;
    game.Initialize(glfwd::ApplicationCreateInfo{});
    game.Run();

    return 0;
}
