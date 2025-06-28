#include "BaseApplication.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "Log/Logger.h"
#include <iostream>
#include <string>

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
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }
    }

    void BaseApplication::Shutdown()
    {
        // Default implementation can be empty or provide basic shutdown logic
    }

    void BaseApplication::Run()
    {
        // Get the GLFW window pointer from Initialize method
        GLFWwindow* window = glfwGetCurrentContext();
        if (!window) {
            std::cout << "Error: No active GLFW window context found!" << std::endl;
            return;
        }

        // Set up variables for delta time calculation
        float lastFrameTime = 0.0f;
        float currentFrameTime = 0.0f;
        float deltaTime = 0.0f;

        // Main game loop
        while (!glfwWindowShouldClose(window) && !ShouldExit())
        {
            // Calculate delta time
            currentFrameTime = static_cast<float>(glfwGetTime());
            deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            // Process input
            ::processInput(window);

            // Update game logic
            Update(deltaTime);

            // Render
            // Clear the screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap buffers and poll IO events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Clean up GLFW resources
        glfwTerminate();
    }

    void BaseApplication::Update(float deltaTime)
    {
        // Base implementation - can be overridden by derived classes
        // This method is called every frame with the time elapsed since the last frame
        
        // You might want to add:
        // 1. Physics updates
        // 2. AI updates
        // 3. Game logic updates
        // 4. Animation updates
        
        // Example:
        // std::cout << "Frame time: " << deltaTime * 1000.0f << " ms" << std::endl;
    }

    const char *BaseApplication::GetName() const
    {
        return "BaseApplication";
    }

    bool BaseApplication::ShouldExit()
    {
        return false; // Default behavior is to not exit
    }
} // namespace framework
