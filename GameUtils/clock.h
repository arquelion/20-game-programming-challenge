#pragma once
#include <chrono>

class Clock
{
public:
    typedef std::chrono::milliseconds          duration;
    typedef duration::rep                      rep;
    typedef duration::period                   period;
    typedef std::chrono::time_point<Clock>     time_point;
    static const bool is_steady = true;

    time_point now() const noexcept
    {
        using namespace std::chrono;
        return time_point(duration_cast<duration>(
            (isPaused ? pauseStart : steady_clock::now()).time_since_epoch() - timePaused));
    }

    void pause() noexcept
    {
        if (!isPaused)
        {
            pauseStart = std::chrono::steady_clock::now();
            isPaused = true;
        }
    }

    void resume() noexcept
    {
        if (isPaused)
        {
            timePaused += duration_cast<duration>(std::chrono::steady_clock::now() - pauseStart);
            isPaused = false;
        }
    }

private:
    bool isPaused = false;

    duration timePaused;
    std::chrono::steady_clock::time_point pauseStart;
};
