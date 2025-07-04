#include "BaseApplication.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Core/Input.h"
#include "Framework/Core/ResLoader.h"
#include "Framework/Performance/FrameRateManager.h"
#include "Framework/Performance/FrameRateMonitor.h"
#include "Framework/Render/RenderComponent.h"
#include "Framework/Render/Renderer.h"

namespace framework
{
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;
    void BaseApplication::Initialize()
    {

        // Initialize the logger
        Logger::Init();
        ResLoader::GetInstance().Initialize("Res/");

        // Renderer: initialize and configure
        // ------------------------------
        renderer = new Renderer();
        renderer->Initialize();
        renderer->SetViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        Input::GetInstance().Initialize(glfwGetCurrentContext());
        
        // 初始化帧率管理器
        FrameRateManager::GetInstance().Initialize();
        // 默认设置为固定帧率策略
        FrameRateManager::GetInstance().SetFrameRateStrategy(FrameRateStrategy::FixedTarget);
        FrameRateManager::GetInstance().SetTargetFrameRate(60);
    }

    void BaseApplication::HandleInput()
    {
        static GLFWwindow *window = glfwGetCurrentContext();
        if (Input::GetInstance().GetKeyDown(GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void BaseApplication::InitScenes() {}

    void BaseApplication::SetInitialScene(const std::string &sceneName)
    {
        if (SceneManager::GetInstance().SetActiveScene(sceneName))
        {
            SceneManager::GetInstance().InitializeActiveScene();
        }
    }

    void BaseApplication::Shutdown()
    {
        SceneManager::GetInstance().ShutdownActiveScene();
    }

    void BaseApplication::Run()
    {
        // Get the GLFW window pointer from Initialize method
        GLFWwindow *window = glfwGetCurrentContext();
        if (!window)
        {
            Logger::Error("No active GLFW window context found!");
            return;
        }
        
        // Set up variables for delta time calculation
        float lastFrameTime = 0.0f;
        float currentFrameTime = 0.0f;
        float deltaTime = 0.0f;
        InitScenes();

        // Main game loop
        while (!ShouldExit())
        {
            // 记录这一帧开始的时间
            double frameStartTime = glfwGetTime();
            
            // Calculate delta time
            currentFrameTime = static_cast<float>(frameStartTime);
            deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            // Process input
            Input::GetInstance().Update();

            // 处理子类的输入
            HandleInput();

            // Update game logic
            Update(deltaTime);
            
            // 更新帧率管理器
            FrameRateManager::GetInstance().Update(deltaTime);


            // Render
            // Clear the screen
            renderer->Clear();

            // 渲染当前场景
            SceneManager::GetInstance().RenderActiveScene(renderer);

            // Swap buffers and poll IO events
            renderer->SwapBuffers();
            
            // 根据当前帧率策略等待必要的时间
            if (FrameRateManager::GetInstance().GetFrameRateStrategy() == FrameRateStrategy::FixedTarget ||
                FrameRateManager::GetInstance().GetFrameRateStrategy() == FrameRateStrategy::PowerSave)
            {
                // 获取目标帧率
                int targetFPS = (FrameRateManager::GetInstance().GetFrameRateStrategy() == FrameRateStrategy::PowerSave) 
                    ? FrameRateManager::GetInstance().GetPowerSaveFrameRate()
                    : FrameRateManager::GetInstance().GetTargetFrameRate();
                
                if (targetFPS > 0)
                {
                    // 计算这一帧应该花费的时间
                    double targetFrameTime = 1.0 / targetFPS;
                    
                    // 计算已经过去的时间
                    double frameEndTime = glfwGetTime();
                    double elapsedTime = frameEndTime - frameStartTime;
                    
                    // 如果还有时间，等待剩余时间
                    if (elapsedTime < targetFrameTime)
                    {
                        double sleepTime = targetFrameTime - elapsedTime;
                        
                        // 使用glfwWaitEventsTimeout进行睡眠
                        // 这比Sleep函数更精确，并且允许处理事件
                        glfwWaitEventsTimeout(sleepTime);
                    }
                }
            }
        }

        // 关闭当前场景
        SceneManager::GetInstance().ShutdownActiveScene();

        // Clean up GLFW resources
        glfwTerminate();
    }

    void BaseApplication::Update(float deltaTime)
    {
        // 更新当前场景
        SceneManager::GetInstance().UpdateActiveScene(deltaTime);
    }

    const char *BaseApplication::GetName() const
    {
        return "BaseApplication";
    }

    bool BaseApplication::ShouldExit()
    {
        static GLFWwindow *window = glfwGetCurrentContext();
        return glfwWindowShouldClose(window);
    }

    void BaseApplication::SetTargetFrameRate(int frameRate)
    {
        FrameRateManager::GetInstance().SetTargetFrameRate(frameRate);
    }

    int BaseApplication::GetTargetFrameRate() const
    {
        return FrameRateManager::GetInstance().GetTargetFrameRate();
    }

    void BaseApplication::SetVSyncEnabled(bool enabled)
    {
        FrameRateManager::GetInstance().SetVSyncEnabled(enabled);
    }

    bool BaseApplication::IsVSyncEnabled() const
    {
        return FrameRateManager::GetInstance().IsVSyncEnabled();
    }
    
    void BaseApplication::SetFrameRateStrategy(FrameRateStrategy strategy)
    {
        FrameRateManager::GetInstance().SetFrameRateStrategy(strategy);
    }
    
    float BaseApplication::GetCurrentFPS() const
    {
        return FrameRateMonitor::GetInstance().GetCurrentFPS();
    }

} // namespace framework
