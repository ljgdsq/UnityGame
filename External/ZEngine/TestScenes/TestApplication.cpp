#include "TestScenes/TestApplication.h"
#include "TestScenes/TriangleTestScene.h"
#include "TestScenes/QuadTestScene.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Core/Input.h"
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
    // 使用Input系统处理场景切换
    
    // 按1键切换到三角形场景
    if (Input::GetKeyDown(GLFW_KEY_1)) {
        SceneManager::GetInstance().SetActiveScene("TriangleTest");
        SceneManager::GetInstance().InitializeActiveScene();
        Logger::Log("Switched to Triangle Test Scene");
    }
    
    // 按2键切换到四边形场景
    if (Input::GetKeyDown(GLFW_KEY_2)) {
        SceneManager::GetInstance().SetActiveScene("QuadTest");
        SceneManager::GetInstance().InitializeActiveScene();
        Logger::Log("Switched to Quad Test Scene");
    }

    // 按3键切换到线框渲染模式
    if (Input::GetKeyDown(GLFW_KEY_3)) {
        Logger::Log("Switched to wireframe rendering mode");
        renderer->SetPolygonMode(PolygonMode::Line);
    }

    // 按4键切换到填充渲染模式
    if (Input::GetKeyDown(GLFW_KEY_4)) {
        Logger::Log("Switched to filled rendering mode");
        renderer->SetPolygonMode(PolygonMode::Fill);
    }
    
    // 可以在这里添加更多测试按键处理
}

} // namespace framework
