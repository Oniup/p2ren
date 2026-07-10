#include "p2ren_core/application.h"

#include <SDL3/SDL_events.h>
#include <fmt/base.h>

namespace p2ren {

void Application::Initialize(const ApplicationDescriptor& descriptor)
{
    m_RenderHardwareContext.Initialize(descriptor.Renderer);

    if (descriptor.Window.Resolution == WindowResolution::Custom)
    {
        m_Window = Window(descriptor.Window.Title,
                          descriptor.Window.Width,
                          descriptor.Window.Height,
                          descriptor.Window.Flags);
    }
    else
    {
        m_Window =
            Window(descriptor.Window.Title, descriptor.Window.Resolution, descriptor.Window.Flags);
    }

    m_RenderHardwareContext.InitializeBackend(m_Window, descriptor.Renderer);
}

void Application::AttachPlugins()
{
}

void Application::Run()
{
    SDL_Event event;
    while (m_Window.IsOpen())
    {
        while (SDL_PollEvent(&event))
        {
            // TODO: make event dispatcher handle this
            m_Window.HandleSDLEvents(event);
        }

        m_RenderHardwareContext.SwapBuffers(m_Window);
    }
}

} // namespace p2ren
