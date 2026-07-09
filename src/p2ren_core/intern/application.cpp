#include "p2ren_core/application.h"

#include <fmt/base.h>

namespace p2ren {

Application::Application()
{
    fmt::print("Creating application\n");
}

Application::~Application()
{
    fmt::print("Destroying application\n");
}

} // namespace p2ren
