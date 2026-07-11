#pragma once

#include "p2ren_core/application.h"

namespace p2ren_example {

class GameProgram final : public p2ren::Application
{
public:
    void AttachEntityDescriptors() override;
};

} // namespace p2ren_example
