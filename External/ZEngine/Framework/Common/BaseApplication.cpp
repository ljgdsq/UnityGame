#include "BaseApplication.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Core/Input.h"
#include "Framework/Render/Renderer.h"
#include "Framework/Core/ResLoader.h"

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
            // Calculate delta time
            currentFrameTime = static_cast<float>(glfwGetTime());
            deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            // Process input
            Input::GetInstance().Update();

            // 处理子类的输入
            HandleInput();

            // Update game logic
            Update(deltaTime);

            // Render
            // Clear the screen

            renderer->Clear();

            // 渲染当前场景
            SceneManager::GetInstance().RenderActiveScene();

            // Swap buffers and poll IO events
            renderer->SwapBuffers();
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
} // namespace framework
