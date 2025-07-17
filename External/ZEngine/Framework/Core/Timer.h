#pragma once
#include <chrono>
#include "Framework/Common/Define.h"
namespace framework
{

    class Timer
    {
        SINGLETON_CLASS(Timer);

    public:

        void Update()
        {
            auto now = std::chrono::steady_clock::now();
            deltaTime = std::chrono::duration<float>(now - m_last).count();
            m_last = now;
        }

        void Initialize()
        {
            m_start = std::chrono::steady_clock::now();
            m_last = m_start;
        }

        float GetDeltaTime()
        {
            return deltaTime;
        }

        float GetElapsedTime() const
        {
            auto now = std::chrono::steady_clock::now();
            return std::chrono::duration<float>(now - m_start).count();
        }

    private:
        std::chrono::steady_clock::time_point m_start;
        std::chrono::steady_clock::time_point m_last;
        float deltaTime;
    };

} // namespace framework
