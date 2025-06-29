#include "BaseApplication.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Framework/Log/Logger.h"
#include "Framework/Core/SceneManager.h"

// Callback functions for GLFW
static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

namespace framework
{
    static const unsigned int SCR_WIDTH = 800;
    static const unsigned int SCR_HEIGHT = 600;
    void BaseApplication::Initialize()
    {
        // Initialize the logger
        Logger::Init();
        
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // --------------------
        GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ZEngine", NULL, NULL);
        if (window == NULL)
        {
            Logger::Error("Failed to create GLFW window");
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::Error("Failed to initialize GLAD");
            return;
        }
    }

    void BaseApplication::HandleInput(){}
    void BaseApplication::InitScenes(){}

    void BaseApplication::SetInitialScene(const std::string& sceneName)
    {
        if (SceneManager::GetInstance().SetActiveScene(sceneName)) {
            SceneManager::GetInstance().InitializeActiveScene();
        }
    }

    void BaseApplication::Shutdown()
    {
        // 确保场景已关闭
        SceneManager::GetInstance().ShutdownActiveScene();
    }

    void BaseApplication::Run()
    {
        // Get the GLFW window pointer from Initialize method
        GLFWwindow* window = glfwGetCurrentContext();
        if (!window) {
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
            ::processInput(window);
            
            // 处理子类的输入
            HandleInput();

            // Update game logic
            Update(deltaTime);

            // Render
            // Clear the screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // 渲染当前场景
            SceneManager::GetInstance().RenderActiveScene();

            // Swap buffers and poll IO events
            glfwSwapBuffers(window);
            glfwPollEvents();
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
        static GLFWwindow* window = glfwGetCurrentContext();
        return glfwWindowShouldClose(window) ;
    }
} // namespace framework
