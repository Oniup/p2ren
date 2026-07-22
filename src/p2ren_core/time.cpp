#include "p2ren_core/time.h"

#include <SDL3/SDL_timer.h>

namespace p2ren {

Timer::Timer(uint64_t duration)
    : m_Duration(duration)
{
}

bool Timer::Finished() const
{
    uint64_t current_time = SDL_GetTicks();
    return current_time >= m_StartTime + m_Duration;
}

void Timer::SetDuration(uint64_t duration)
{
    m_Duration = duration;
}

void Timer::Start()
{
    m_StartTime = SDL_GetTicks();
}

double Timestep::GetElapsedSeconds()
{
    uint64_t counter = SDL_GetTicks();
    return counter / 1000.0;
}

uint64_t Timestep::GetElapsedMilliseconds()
{
    return SDL_GetTicks();
}

void Timestep::CalculateDeltaTime()
{
    uint64_t current_time = SDL_GetPerformanceCounter();
    m_DeltaTime = (float)(current_time - m_LastTime) / (float)SDL_GetPerformanceFrequency();
    m_LastTime  = current_time;
}

} // namespace p2ren
