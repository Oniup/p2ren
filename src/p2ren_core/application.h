#pragma once

#include "p2ren_core/application_descriptor.h"
#include "p2ren_core/resource_manager.h"
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

    Window          m_Window;
    ResourceManager m_ResourceManager;
    Renderer        m_RenderHardwareContext;
};

} // namespace p2ren
