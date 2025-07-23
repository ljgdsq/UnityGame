#include "BaseApplication.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Core/Input.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Performance/FrameRateManager.h"
#include "Framework/Performance/FrameRateMonitor.h"
#include "Framework/Graphic/RenderComponent.h"
#include "Framework/Graphic/Renderer.h"
#include "Framework/Core/Timer.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Asset/TextureLoader.h"
#include "Framework/Asset/ObjMeshLoader.h"
// platform specific includes
#include "Framework/Window/Platform/GLFWWindow.h"

namespace framework
{
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;

    BaseApplication::BaseApplication()
        : m_renderer(nullptr), m_window(nullptr)
    {
        // Constructor implementation
        Logger::Log("BaseApplication constructed");
        Timer::GetInstance().Initialize();
    }

    void BaseApplication::Initialize()
    {

        AssetManager::GetInstance().Initialize();
        RegisterDefaultLoader();
        // Initialize the logger
        Logger::Init();

        // Initialize the window
        m_window = new GLFWWindow();
        WindowConfig config;
        config.title = "My Game";
        config.width = SCR_WIDTH;
        config.height = SCR_HEIGHT;
        m_window->Initialize(config);

        // Initialize the renderer
        m_renderer = new Renderer();
        m_renderer->Initialize(m_window, glfwGetProcAddress);

        Input::GetInstance().Initialize(m_window);
        // 初始化帧率管理器
        FrameRateManager::GetInstance().Initialize(m_window);
        // 默认设置为固定帧率策略
        FrameRateManager::GetInstance().SetFrameRateStrategy(FrameRateStrategy::FixedTarget);
        FrameRateManager::GetInstance().SetTargetFrameRate(60);
    }

    void BaseApplication::HandleInput()
    {
        if (Input::GetInstance().GetKeyDown(GLFW_KEY_ESCAPE))
        {
            m_window->SetShouldClose(true);
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

        if (!m_window || !m_window->GetNativeWindowHandle())
        {
            Logger::Error("Window not initialized or no native window handle found!");
            return;
        }

        InitScenes();

        // Main game loop
        while (!ShouldExit())
        {
            m_window->PollEvents();
            Timer::GetInstance().Update();

            float deltaTime = Timer::GetInstance().GetDeltaTime();
            float frameStartTime = Timer::GetInstance().GetElapsedTime();

            Input::GetInstance().Update();

            // 处理子类的输入
            HandleInput();

            // Update game logic
            Update(deltaTime);

            // 更新帧率管理器
            FrameRateManager::GetInstance().Update(deltaTime);

            BegineFrame();
            // Render
            // Clear the screen
            m_renderer->Clear();

            // 渲染当前场景
            SceneManager::GetInstance().RenderActiveScene(m_renderer);
            EndFrame();
            // Swap buffers
            m_renderer->SwapBuffers();

            FrameRateManager::GetInstance().SleepToNextFrame(frameStartTime);
        }

        // 关闭当前场景
        SceneManager::GetInstance().ShutdownActiveScene();

        m_window->Destroy();
        m_window = nullptr;
    }

    void BaseApplication::Update(float deltaTime)
    {
        OnUpdate(deltaTime);
        // 更新当前场景
        SceneManager::GetInstance().UpdateActiveScene(deltaTime);
    }

    const char *BaseApplication::GetName() const
    {
        return "BaseApplication";
    }

    bool BaseApplication::ShouldExit()
    {
        return m_window && m_window->ShouldClose();
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

    void RegisterDefaultLoader()
    {
        // Register default asset loaders
        AssetManager::GetInstance().RegisterLoader(std::make_shared<TextureLoader>());
        AssetManager::GetInstance().RegisterLoader(std::make_shared<ObjMeshLoader>());
    }

} // namespace framework
