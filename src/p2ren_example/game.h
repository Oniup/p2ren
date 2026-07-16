#pragma once

#include "p2ren_engine/application.h"

namespace p2ren_example {

class Game : public p2ren::Application
{
public:
    void AttachEntityDescriptors() override;
};

} // namespace p2ren_example
