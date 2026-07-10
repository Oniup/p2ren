#pragma once

#include <ankerl/unordered_dense.h>

#include <memory>

#include "p2ren_core/application_descriptor.h"
#include "p2ren_core/plugin.h"
#include "p2ren_core/type_info.h"
#include "p2ren_core/window.h"
#include "p2ren_render_hardware/context.h"

namespace p2ren {

class Application
{
public:
    virtual ~Application() = default;
    void Initialize(const ApplicationDescriptor& descriptor);

    virtual void AttachPlugins();

    void Run();

    template <DerivedPlugin T>
    T* GetPlugin()
    {
        uint64_t key = TypeInfo<T>::GetHash();
        if (m_Plugins.contains(key))
        {
            return m_Plugins.at(key);
        }
        return nullptr;
    }

    template <DerivedPlugin T>
    const T* GetPlugin() const
    {
        uint64_t key = TypeInfo<T>::GetHash();
        if (m_Plugins.contains(key))
        {
            return m_Plugins.at(key);
        }
        return nullptr;
    }

private:
    void InitializeMandetoryPlugins();

    RHI_Context m_RenderHardwareContext;
    Window      m_Window;

    ankerl::unordered_dense::map<uint64_t, std::unique_ptr<Plugin*>> m_Plugins;
};

} // namespace p2ren
