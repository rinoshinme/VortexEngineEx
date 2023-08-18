#pragma once


#include <chrono>

namespace vortex
{
    class Timer
    {

    };

    class FpsCounter
    {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
        int m_NumFrames;
        float m_PrevFps;

    public:
        FpsCounter();

        float Update();
    };
}

