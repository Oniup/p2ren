#pragma once

#include <string>

#include "p2ren_engine/create_info.h"

namespace p2ren {

class ForwardRenderer;
class Window;
class ResourceManager;
class Timestep;

class Application
{
public:
    static std::string_view GetBasePath();

    Application() = default;
    virtual ~Application();
    void Initialize(const ApplicationCreateInfo& create_info);

    virtual void AttachActorCreateInfos() = 0;
    virtual void OnStartup() {}

    void Run();

protected:
    /// Iterate over all directories and check if its the 'assets' folder, otherwise check the
    /// parent path until successfully found the directory. Will crash if can't find.
    static std::string FindAssetDirectory();

    ForwardRenderer* m_Renderer        = nullptr;
    Window*          m_Window          = nullptr;
    ResourceManager* m_ResourceManager = nullptr;
};

} // namespace p2ren
