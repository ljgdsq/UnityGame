// #include "FrameRateMonitor.h"
#include "Framework/Performance/FrameRateMonitor.h"
#include <limits>
#include <memory>
#include <algorithm>

namespace framework
{
    std::unique_ptr<FrameRateMonitor> FrameRateMonitor::s_instance = nullptr;

    FrameRateMonitor& FrameRateMonitor::GetInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<FrameRateMonitor>(new FrameRateMonitor());
        }
        return *s_instance;
    }

    FrameRateMonitor::FrameRateMonitor()
        : m_enabled(true),
          m_currentFPS(0.0f),
          m_averageFPS(0.0f),
          m_maxFPS(0.0f),
          m_minFPS(std::numeric_limits<float>::max()),
          m_frameTimeAccumulator(0.0f),
          m_frameCount(0),
          m_timeSinceUpdate(0.0f)
    {
    }

    void FrameRateMonitor::Initialize()
    {
        Reset();
    }

    void FrameRateMonitor::Update(float deltaTime)
    {
        if (!m_enabled || deltaTime <= 0.0f)
            return;

        m_frameCount++;
        m_frameTimeAccumulator += deltaTime;
        m_timeSinceUpdate += deltaTime;

        // 更新FPS计算（每m_updateInterval秒更新一次）
        if (m_timeSinceUpdate >= m_updateInterval)
        {
            // 计算当前FPS
            m_currentFPS = m_frameCount / m_timeSinceUpdate;
            
            // 更新最大/最小FPS
            m_maxFPS = std::max(m_maxFPS, m_currentFPS);
            m_minFPS = std::min(m_minFPS, m_currentFPS);
            
            // 重置统计周期
            m_frameCount = 0;
            m_timeSinceUpdate = 0.0f;
        }

        // 计算平均FPS
        if (m_frameTimeAccumulator > 0.0f)
        {
            m_averageFPS = static_cast<float>(m_frameCount) / m_frameTimeAccumulator;
        }
    }

    float FrameRateMonitor::GetCurrentFPS() const
    {
        return m_currentFPS;
    }

    float FrameRateMonitor::GetAverageFPS() const
    {
        return m_averageFPS;
    }

    float FrameRateMonitor::GetMaxFPS() const
    {
        return m_maxFPS;
    }

    float FrameRateMonitor::GetMinFPS() const
    {
        return m_minFPS;
    }

    void FrameRateMonitor::Reset()
    {
        m_currentFPS = 0.0f;
        m_averageFPS = 0.0f;
        m_maxFPS = 0.0f;
        m_minFPS = std::numeric_limits<float>::max();
        m_frameTimeAccumulator = 0.0f;
        m_frameCount = 0;
        m_timeSinceUpdate = 0.0f;
    }

    void FrameRateMonitor::SetEnabled(bool enabled)
    {
        m_enabled = enabled;
        if (m_enabled)
        {
            Reset();
        }
    }

    bool FrameRateMonitor::IsEnabled() const
    {
        return m_enabled;
    }
}
