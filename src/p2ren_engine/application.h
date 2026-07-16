#pragma once

#include <string>

#include "p2ren_engine/create_info.h"

namespace p2ren {

class ForwardRenderer;
class Window;
class ResourceManager;

class Application
{
public:
    Application();
    virtual ~Application();
    void Initialize(const ApplicationCreateInfo& create_info);

    virtual void AttachActorCreateInfos() = 0;

    void Run();

    static Window*          GetWindow() { return s_Application->m_Window; }
    static ResourceManager* GetResourceManager() { return s_Application->m_ResourceManager; }
    static std::string_view GetBasePath();

private:
    /// Iterate over all directories and check if its the 'assets' folder, otherwise check the
    /// parent path until successfully found the directory. Will crash if can't find.
    static std::string FindAssetDirectory();

    static Application* s_Application;

    ForwardRenderer* m_Renderer        = nullptr;
    Window*          m_Window          = nullptr;
    ResourceManager* m_ResourceManager = nullptr;
};

} // namespace p2ren
