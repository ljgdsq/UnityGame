#pragma once
#include <memory>
#include <limits>

namespace framework
{
    /**
     * @brief 帧率监控类，用于测量和显示游戏的帧率
     */
    class FrameRateMonitor
    {
    public:
        static FrameRateMonitor& GetInstance();

        // 初始化帧率监控器
        void Initialize();

        // 更新帧率统计（每帧调用）
        void Update(float deltaTime);

        // 获取当前帧率
        float GetCurrentFPS() const;

        // 获取平均帧率
        float GetAverageFPS() const;

        // 获取最高帧率
        float GetMaxFPS() const;

        // 获取最低帧率
        float GetMinFPS() const;

        // 重置统计数据
        void Reset();

        // 启用/禁用帧率监控
        void SetEnabled(bool enabled);

        // 是否启用帧率监控
        bool IsEnabled() const;

    private:
        FrameRateMonitor();
        FrameRateMonitor(const FrameRateMonitor&) = delete;
        FrameRateMonitor& operator=(const FrameRateMonitor&) = delete;

    public:
        // 析构函数必须是公有的，这样std::unique_ptr才能正确删除对象
        ~FrameRateMonitor() = default;

    private:
        static std::unique_ptr<FrameRateMonitor> s_instance;

        bool m_enabled = true;
        float m_currentFPS = 0.0f;
        float m_averageFPS = 0.0f;
        float m_maxFPS = 0.0f;
        float m_minFPS = std::numeric_limits<float>::max();
        
        // 用于计算平均帧率
        float m_frameTimeAccumulator = 0.0f;
        int m_frameCount = 0;
        
        // 计算FPS的时间窗口（秒）
        const float m_updateInterval = 0.5f;
        float m_timeSinceUpdate = 0.0f;
    };
}
