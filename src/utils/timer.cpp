#include "timer.h"


namespace vortex
{
    FpsCounter::FpsCounter()
    {
        m_Start = std::chrono::high_resolution_clock::now();
        m_NumFrames = 0;
        m_PrevFps = 0.0f;
    }

    float FpsCounter::Update()
    {
        auto current = std::chrono::high_resolution_clock::now();
        // time difference in seconds
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(current - m_Start).count() * 0.001f * 0.001f;
        m_NumFrames += 1;
        if (diff > 0.25)
        {
            float fps = 1.0f / (diff / m_NumFrames);
            m_PrevFps = fps;
            m_NumFrames = 0;
            m_Start = current;
            return fps;
        }
        return m_PrevFps;
    }
}
