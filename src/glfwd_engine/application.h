#pragma once

#include <string>

#include "glfwd_engine/create_info.h"

namespace glfwd {

class ForwardRenderer;
class Window;
class ResourceManager;
class Timestep;
class RenderQueue;

class Application
{
public:
    static std::string_view GetBasePath();

    Application() = default;
    virtual ~Application();
    void Initialize(const ApplicationCreateInfo& create_info);

    void Run();

protected:
    /// Iterate over all directories and check if its the 'assets' folder, otherwise check the
    /// parent path until successfully found the directory. Will crash if can't find.
    static std::string FindAssetDirectory();

    virtual void OnInitialize()                                 = 0;
    virtual void OnUpdate(const Timestep& timestep)             = 0;
    virtual void OnLateUpdate(const Timestep& timestep)         = 0;
    virtual void SubmitToRenderQueue(RenderQueue* render_queue) = 0;

    ForwardRenderer* m_Renderer        = nullptr;
    Window*          m_Window          = nullptr;
    ResourceManager* m_ResourceManager = nullptr;
};

} // namespace glfwd
