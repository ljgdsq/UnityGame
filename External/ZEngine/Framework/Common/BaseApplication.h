#pragma once
#include <string>
#include "Framework/Interface/IApplication.hpp"
#include "Framework/Render/Renderer.h"
#include "Framework/Core/Input.h"
namespace framework
{
    class BaseApplication : public interface::IApplication
    {
    public:
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
        const char *GetName() const;

        // Check if the application should exit
        bool ShouldExit() override;

    protected:
        // 设置初始场景
        virtual void SetInitialScene(const std::string &sceneName);

        // 处理输入 - 在子类中实现
        virtual void HandleInput();

        virtual void InitScenes();

        protected:
        Renderer * renderer = nullptr;

    };
} // namespace framework
