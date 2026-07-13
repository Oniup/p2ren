#include "p2ren_core/application_descriptor.h"
#include "p2ren_example/program.h"

int main()
{
    p2ren_example::GameProgram game;
    game.Initialize(p2ren::ApplicationDescriptor{});
    game.Run();

    return 0;
}
