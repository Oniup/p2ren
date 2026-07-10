#pragma once

#include <string_view>
#include <type_traits>

namespace p2ren {

class Application;

class Plugin
{
    friend Application;

public:
    virtual std::string_view GetName() = 0;

    virtual void OnInitialize() {}
    virtual void OnDestroy() {}

    virtual void OnUpdate() {}

protected:
    Application*       GetApplication() { return m_Application; }
    const Application* GetApplication() const { return m_Application; }

private:
    class Application* m_Application;
};

template <typename T>
concept DerivedPlugin = std::is_base_of_v<Plugin, T>;

} // namespace p2ren
