#include "p2ren_core/application.h"

int main()
{
    p2ren::Application app;
    app.Initialize(p2ren::ApplicationDescriptor{});
    app.Run();

    return 0;
}
