#include "p2ren_engine/create_info.h"
#include "p2ren_example/game.h"

int main()
{
    p2ren_example::Game game;
    game.Initialize(p2ren::ApplicationCreateInfo{
        .Window =
            {
                .Resolution = p2ren::WindowResolution::Auto,
            },
    });
    game.Run();

    return 0;
}
