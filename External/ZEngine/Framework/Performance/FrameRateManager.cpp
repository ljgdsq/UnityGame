#include "Framework/Performance/FrameRateManager.h"
#include "Framework/Performance/FrameRateMonitor.h"
#include "Framework/Log/Logger.h"
#include "glfw/glfw3.h"
#include <memory>
#include <algorithm>

namespace framework
{
    std::unique_ptr<FrameRateManager> FrameRateManager::s_instance = nullptr;

    FrameRateManager& FrameRateManager::GetInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<FrameRateManager>(new FrameRateManager());
        }
        return *s_instance;
    }

    FrameRateManager::FrameRateManager()
        : m_strategy(FrameRateStrategy::FixedTarget),
          m_targetFrameRate(60),
          m_vsyncEnabled(false),
          m_powerSaveFrameRate(30),
          m_minAdaptiveFrameRate(30),
          m_maxAdaptiveFrameRate(144),
          m_adaptationSpeed(0.1f),
          m_currentAdaptiveTarget(60)
    {
    }

    void FrameRateManager::Initialize()
    {
        // 初始化帧率监控器
        FrameRateMonitor::GetInstance().Initialize();
        
        // 应用初始策略
        ApplyStrategy();
        
        Logger::Log("Frame Rate Manager initialized with strategy: {}", GetStrategyDescription());
    }

    void FrameRateManager::Update(float deltaTime)
    {
        // 更新帧率监控
        FrameRateMonitor::GetInstance().Update(deltaTime);
        
        // 如果使用自适应帧率，根据性能调整帧率
        if (m_strategy == FrameRateStrategy::AdaptiveFrameRate)
        {
            float currentFPS = FrameRateMonitor::GetInstance().GetCurrentFPS();
            
            // 如果当前帧率低于目标的90%，降低目标帧率
            if (currentFPS < m_currentAdaptiveTarget * 0.9f && m_currentAdaptiveTarget > m_minAdaptiveFrameRate)
            {
                m_currentAdaptiveTarget = std::max(m_minAdaptiveFrameRate, 
                    m_currentAdaptiveTarget - static_cast<int>(m_adaptationSpeed * m_currentAdaptiveTarget));
                
                SetTargetFrameRate(m_currentAdaptiveTarget);
                Logger::Log("Adaptive frame rate decreased to: {}", m_currentAdaptiveTarget);
            }
            // 如果当前帧率高于目标的95%，尝试提高目标帧率
            else if (currentFPS > m_currentAdaptiveTarget * 0.95f && m_currentAdaptiveTarget < m_maxAdaptiveFrameRate)
            {
                m_currentAdaptiveTarget = std::min(m_maxAdaptiveFrameRate,
                    m_currentAdaptiveTarget + static_cast<int>(m_adaptationSpeed * m_currentAdaptiveTarget));
                
                SetTargetFrameRate(m_currentAdaptiveTarget);
                Logger::Log("Adaptive frame rate increased to: {}", m_currentAdaptiveTarget);
            }
        }
    }

    void FrameRateManager::SetFrameRateStrategy(FrameRateStrategy strategy)
    {
        if (m_strategy != strategy)
        {
            m_strategy = strategy;
            ApplyStrategy();
            Logger::Log("Frame rate strategy changed to: {}", GetStrategyDescription());
        }
    }

    FrameRateStrategy FrameRateManager::GetFrameRateStrategy() const
    {
        return m_strategy;
    }

    void FrameRateManager::SetTargetFrameRate(int frameRate)
    {
        // 确保帧率为正数或零（零表示不限制）
        m_targetFrameRate = frameRate > 0 ? frameRate : 0;
        
        // 如果当前策略是固定帧率，应用新设置
        if (m_strategy == FrameRateStrategy::FixedTarget)
        {
            ApplyStrategy();
        }
        
        Logger::Log("Target frame rate set to: {}", m_targetFrameRate);
    }

    int FrameRateManager::GetTargetFrameRate() const
    {
        return m_targetFrameRate;
    }

    void FrameRateManager::SetVSyncEnabled(bool enabled)
    {
        if (m_vsyncEnabled != enabled)
        {
            m_vsyncEnabled = enabled;
            
            // 应用垂直同步设置
            GLFWwindow* window = glfwGetCurrentContext();
            if (window)
            {
                glfwSwapInterval(m_vsyncEnabled ? 1 : 0);
                Logger::Log("VSync {}", m_vsyncEnabled ? "enabled" : "disabled");
            }
            
            // 如果当前策略是垂直同步，确保策略正确应用
            if (m_strategy == FrameRateStrategy::VSync)
            {
                ApplyStrategy();
            }
        }
    }

    bool FrameRateManager::IsVSyncEnabled() const
    {
        return m_vsyncEnabled;
    }

    void FrameRateManager::SetPowerSaveFrameRate(int frameRate)
    {
        // 省电模式帧率至少为15，最高不超过60
        m_powerSaveFrameRate = std::min(60, std::max(15, frameRate));
        
        // 如果当前策略是省电模式，应用新设置
        if (m_strategy == FrameRateStrategy::PowerSave)
        {
            ApplyStrategy();
        }
        
        Logger::Log("Power save frame rate set to: {}", m_powerSaveFrameRate);
    }

    int FrameRateManager::GetPowerSaveFrameRate() const
    {
        return m_powerSaveFrameRate;
    }

    std::string FrameRateManager::GetStrategyDescription() const
    {
        switch (m_strategy)
        {
        case FrameRateStrategy::Unlimited:
            return "Unlimited";
        case FrameRateStrategy::FixedTarget:
            return "Fixed Target (" + std::to_string(m_targetFrameRate) + " FPS)";
        case FrameRateStrategy::VSync:
            return "VSync";
        case FrameRateStrategy::AdaptiveFrameRate:
            return "Adaptive (" + std::to_string(m_minAdaptiveFrameRate) + "-" + 
                   std::to_string(m_maxAdaptiveFrameRate) + " FPS)";
        case FrameRateStrategy::PowerSave:
            return "Power Save (" + std::to_string(m_powerSaveFrameRate) + " FPS)";
        default:
            return "Unknown";
        }
    }

    void FrameRateManager::ApplyStrategy()
    {
        GLFWwindow* window = glfwGetCurrentContext();
        if (!window)
        {
            Logger::Warn("Cannot apply frame rate strategy: No active GLFW window");
            return;
        }
        
        // 根据策略应用相应设置
        switch (m_strategy)
        {
        case FrameRateStrategy::Unlimited:
            // 关闭垂直同步，不限制帧率
            glfwSwapInterval(0);
            m_vsyncEnabled = false;
            break;
            
        case FrameRateStrategy::FixedTarget:
            // 关闭垂直同步，使用目标帧率
            glfwSwapInterval(0);
            m_vsyncEnabled = false;
            break;
            
        case FrameRateStrategy::VSync:
            // 启用垂直同步
            glfwSwapInterval(1);
            m_vsyncEnabled = true;
            break;
            
        case FrameRateStrategy::AdaptiveFrameRate:
            // 关闭垂直同步，使用自适应帧率
            glfwSwapInterval(0);
            m_vsyncEnabled = false;
            m_currentAdaptiveTarget = m_targetFrameRate;
            break;
            
        case FrameRateStrategy::PowerSave:
            // 关闭垂直同步，使用省电模式帧率
            glfwSwapInterval(0);
            m_vsyncEnabled = false;
            break;
        }
        
        Logger::Log("Applied frame rate strategy: {}", GetStrategyDescription());
    }
}
