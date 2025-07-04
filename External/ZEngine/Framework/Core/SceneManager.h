#pragma once

#include "Framework/Core/Scene.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace framework {
class Renderer; // 前向声明渲染器
class SceneManager {
private:
    std::unordered_map<std::string, ScenePtr> m_scenes;
    ScenePtr m_activeScene = nullptr;
    std::string m_activeSceneName;
    
    // 单例实例
    static std::unique_ptr<SceneManager> s_instance;

public:
    SceneManager() = default;
    ~SceneManager() = default;
    
    // 获取单例实例
    static SceneManager& GetInstance();
    
    // 添加场景
    void AddScene(const std::string& name, ScenePtr scene);
    
    // 获取场景
    ScenePtr GetScene(const std::string& name);
    
    // 设置活动场景
    bool SetActiveScene(const std::string& name);
    
    // 获取活动场景
    ScenePtr GetActiveScene() const;
    
    // 获取活动场景名称
    const std::string& GetActiveSceneName() const;
    
    // 初始化活动场景
    void InitializeActiveScene();
    
    // 更新活动场景
    void UpdateActiveScene(float deltaTime);
    
    // 渲染活动场景
    void RenderActiveScene(Renderer* renderer);
    
    // 关闭活动场景
    void ShutdownActiveScene();
};

} // namespace framework
