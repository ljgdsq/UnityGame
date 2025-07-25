#include "Framework/Core/SceneManager.h"
#include "Framework/Log/Logger.h"
#include "Framework/Graphic/Renderer.h"
#include "Framework/Common/BaseApplication.h"
namespace framework
{

    void SceneManager::AddScene(const std::string &name, Scene *scene)
    {
        m_scenes[name] = scene;
        Logger::Log("Scene '{}' added to SceneManager", name);
    }

    Scene *SceneManager::GetScene(const std::string &name)
    {
        auto it = m_scenes.find(name);
        if (it != m_scenes.end())
        {
            return it->second;
        }
        Logger::Warn("Scene '{}' not found in SceneManager", name);
        return nullptr;
    }

    bool SceneManager::SetActiveScene(const std::string &name)
    {
        auto it = m_scenes.find(name);
        if (it != m_scenes.end())
        {
            // 如果已经有活动场景，先关闭它
            if (m_activeScene)
            {
                m_activeScene->Shutdown();
            }

            m_activeScene = it->second;
            m_activeSceneName = name;
            Logger::Log("Active scene set to '{}'", name);
            BaseApplication::GetInstance()->SetTitle("ZEngine - Running scene : " + name);
            return true;
        }

        Logger::Error("Failed to set active scene: '{}' not found", name);
        return false;
    }

    Scene *SceneManager::GetActiveScene() const
    {
        return m_activeScene;
    }

    const std::string &SceneManager::GetActiveSceneName() const
    {
        return m_activeSceneName;
    }

    void SceneManager::InitializeActiveScene()
    {
        if (m_activeScene)
        {
            Logger::Log("Initializing scene '{}'", m_activeSceneName);
            m_activeScene->Initialize();
        }
        else
        {
            Logger::Warn("Cannot initialize: No active scene");
        }
    }

    void SceneManager::UpdateActiveScene(float deltaTime)
    {
        if (m_activeScene)
        {
            m_activeScene->Update(deltaTime);
        }
    }

    void SceneManager::RenderActiveScene(Renderer *renderer)
    {
        if (m_activeScene)
        {
            m_activeScene->Render(renderer);
        }
    }

    void SceneManager::ShutdownActiveScene()
    {
        if (m_activeScene)
        {
            Logger::Log("Shutting down scene '{}'", m_activeSceneName);
            m_activeScene->Shutdown();
        }
    }

} // namespace framework
