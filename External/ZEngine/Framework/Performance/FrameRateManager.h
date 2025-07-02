#pragma once
#include <string>
#include <memory>

namespace framework
{
    /**
     * @brief 帧率管理策略枚举
     */
    enum class FrameRateStrategy
    {
        Unlimited,          // 不限制帧率
        FixedTarget,        // 固定帧率目标
        VSync,              // 使用垂直同步
        AdaptiveFrameRate,  // 自适应帧率（根据性能动态调整）
        PowerSave           // 省电模式（低帧率）
    };

    /**
     * @brief 帧率管理器类，提供高级帧率管理功能
     */
    class FrameRateManager
    {
    public:
        static FrameRateManager& GetInstance();

        // 初始化帧率管理器
        void Initialize();

        // 更新帧率管理（每帧调用）
        void Update(float deltaTime);

        // 设置帧率策略
        void SetFrameRateStrategy(FrameRateStrategy strategy);

        // 获取当前帧率策略
        FrameRateStrategy GetFrameRateStrategy() const;

        // 设置目标帧率
        void SetTargetFrameRate(int frameRate);

        // 获取目标帧率
        int GetTargetFrameRate() const;

        // 启用/禁用垂直同步
        void SetVSyncEnabled(bool enabled);

        // 是否启用垂直同步
        bool IsVSyncEnabled() const;

        // 设置省电模式的帧率
        void SetPowerSaveFrameRate(int frameRate);

        // 获取省电模式的帧率
        int GetPowerSaveFrameRate() const;

        // 获取当前策略的描述
        std::string GetStrategyDescription() const;

    private:
        FrameRateManager();
        FrameRateManager(const FrameRateManager&) = delete;
        FrameRateManager& operator=(const FrameRateManager&) = delete;

    public:
        // 析构函数必须是公有的，这样std::unique_ptr才能正确删除对象
        ~FrameRateManager() = default;

    private:
        // 应用当前帧率策略
        void ApplyStrategy();

    private:
        static std::unique_ptr<FrameRateManager> s_instance;

        FrameRateStrategy m_strategy = FrameRateStrategy::FixedTarget;
        int m_targetFrameRate = 60;       // 默认目标帧率60fps
        bool m_vsyncEnabled = false;      // 默认不启用垂直同步
        int m_powerSaveFrameRate = 30;    // 省电模式默认30fps
        
        // 自适应帧率相关
        int m_minAdaptiveFrameRate = 30;  // 自适应最低帧率
        int m_maxAdaptiveFrameRate = 144; // 自适应最高帧率
        float m_adaptationSpeed = 0.1f;   // 自适应速度
        int m_currentAdaptiveTarget = 60; // 当前自适应目标
    };
}
