#pragma once

#include <cstdint>

namespace p2ren {

class Timer
{
public:
    Timer(uint64_t duration = 0);

    uint64_t GetStartTime() const { return m_StartTime; }
    uint64_t GetDuration() const { return m_Duration; }

    bool Finished() const;

    void SetDuration(uint64_t duration); /// Duration in milliseconds
    void Start();                        /// Start time when called

private:
    uint64_t m_Duration  = 0;
    uint64_t m_StartTime = 0;
};

class Timestep
{
public:
    /// Get time duration since the program started in seconds
    static double   GetElapsedSeconds();
    static uint64_t GetElapsedMilliseconds();

    /// Get time difference between current and previous frame
    double GetDeltaTime() const { return m_DeltaTime; }

    void CalculateDeltaTime();

private:
    uint64_t m_LastTime  = 0;
    double   m_DeltaTime = 0.0;
};

} // namespace p2ren
