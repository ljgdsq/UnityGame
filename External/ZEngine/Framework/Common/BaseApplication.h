#pragma once
#include <string>
#include "Framework/Interface/IApplication.hpp"
#include "Framework/Window/IWindow.h"
#include "Framework/Graphic/Renderer.h"
#include "Framework/Core/Input.h"
#include "Framework/Performance/FrameRateManager.h"

namespace framework
{
    class BaseApplication : public interface::IApplication
    {
    public:
        BaseApplication();
        virtual ~BaseApplication() = default;

        // Initialize the application
        void Initialize() override;

        // Shutdown the application
        void Shutdown() override;

        // Run the application
        void Run() override;

        // Update the application with delta time
        void Update(float deltaTime) override;

        // Get the name of the application
        const char *GetName() const override;

        // Check if the application should exit
        bool ShouldExit() override;

    protected:
        // 设置初始场景
        virtual void SetInitialScene(const std::string &sceneName);

        // 处理输入 - 在子类中实现
        virtual void HandleInput();

        virtual void InitScenes();

        virtual void OnUpdate(float deltaTime) {};

        // 设置目标帧率 (fps)
        void SetTargetFrameRate(int frameRate);

        // 获取当前目标帧率
        int GetTargetFrameRate() const;

        // 启用/禁用垂直同步 (VSync)
        void SetVSyncEnabled(bool enabled);

        // 检查垂直同步是否启用
        bool IsVSyncEnabled() const;

        // 设置帧率策略
        void SetFrameRateStrategy(FrameRateStrategy strategy);

        // 获取当前帧率
        float GetCurrentFPS() const;

    protected:
        virtual void BegineFrame() {};
        virtual void EndFrame() {};

    protected:
        Renderer *m_renderer = nullptr; // 渲染器实例
        IWindow *m_window = nullptr;    // 窗口实例
    public:
        static BaseApplication *GetInstance() { return m_instance; };
        void SetShouldClose(bool shouldClose); // 设置窗口是否应该关闭
    private:
        static BaseApplication *m_instance; // 单例实例
    };
} // namespace framework
