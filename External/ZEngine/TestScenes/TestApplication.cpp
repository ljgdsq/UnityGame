#include "TestScenes/TestApplication.h"
#include "TestScenes/TriangleTestScene.h"
#include "TestScenes/QuadTestScene.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Log/Logger.h"
#include <GLFW/glfw3.h>

namespace framework {

TestApplication::TestApplication() {
    // 构造函数中不进行场景初始化，留给Initialize调用
}

void TestApplication::InitScenes() {
    // 注册三角形测试场景
    auto triangleScene = std::make_shared<TriangleTestScene>();
    SceneManager::GetInstance().AddScene("TriangleTest", triangleScene);
    
    // 注册四边形测试场景
    auto quadScene = std::make_shared<QuadTestScene>();
    SceneManager::GetInstance().AddScene("QuadTest", quadScene);
    
    // 可以在这里添加更多测试场景
    
    // 设置初始场景
    SetInitialScene("TriangleTest");
    
    // 输出帮助信息
    Logger::Log("Test Application Initialized");
    Logger::Log("Press 1 to switch to Triangle Test Scene");
    Logger::Log("Press 2 to switch to Quad Test Scene");
}

void TestApplication::HandleInput() {
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) {
        return;
    }
    
    // 按1键切换到三角形场景
    static bool key1Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!key1Pressed) {
            SceneManager::GetInstance().SetActiveScene("TriangleTest");
            SceneManager::GetInstance().InitializeActiveScene();
            Logger::Log("Switched to Triangle Test Scene");
            key1Pressed = true;
        }
    } else {
        key1Pressed = false;
    }
    
    // 按2键切换到四边形场景
    static bool key2Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!key2Pressed) {
            SceneManager::GetInstance().SetActiveScene("QuadTest");
            SceneManager::GetInstance().InitializeActiveScene();
            Logger::Log("Switched to Quad Test Scene");
            key2Pressed = true;
        }
    } else {
        key2Pressed = false;
    }
    
    // 可以在这里添加更多测试按键
}

} // namespace framework
