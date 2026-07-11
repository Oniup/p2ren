#pragma once

#include <ankerl/unordered_dense.h>

#include "p2ren_core/application_descriptor.h"
#include "p2ren_core/window.h"
#include "p2ren_renderer/renderer.h"

namespace p2ren {

class Application
{
public:
    virtual ~Application() = default;
    void Initialize(const ApplicationDescriptor& descriptor);

    virtual void AttachEntityDescriptors() = 0;

    void Run();

    Window&       GetWindow() { return m_Window; }
    const Window& GetWindow() const { return m_Window; }

private:
    void InitializeMandetoryPlugins();

    Renderer m_RenderHardwareContext;
    Window   m_Window;
};

} // namespace p2ren
