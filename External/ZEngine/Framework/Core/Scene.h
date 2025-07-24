#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Framework/Log/Logger.h"
#include "Framework/Core/Serializable.h"
namespace framework
{
    class GameObject;      // 前向声明，避免循环依赖
    class RenderComponent; // 前向声明渲染组件
    class Renderer;
    class Scene : public Serializable
    {
    public:
        Scene() = default;
        virtual ~Scene() = default;

        virtual void Initialize() {};
        virtual void Update(float deltaTime);
        virtual void Render(Renderer *renderer);
        virtual void Shutdown() {};

    public:
        // 获取场景中的所有游戏对象
        std::vector<GameObject *> GetAllGameObjects() const
        {
            return m_gameObjects;
        }

        // 编辑器接口：添加和移除 GameObject
        void AddGameObjectFromEditor(GameObject *gameObject) { AddGameObject(gameObject); }
        void RemoveGameObjectFromEditor(GameObject *gameObject) { RemoveGameObject(gameObject); }

    public:
        virtual void AddGameObject(GameObject *gameObject);
        virtual void RemoveGameObject(GameObject *gameObject);

    private:
        // 禁止拷贝和赋值
        Scene(const Scene &) = delete;
        Scene &operator=(const Scene &) = delete;
        std::vector<GameObject *> m_gameObjects;    // 存储场景中的游戏对象
        std::vector<GameObject *> m_newGameObjects; // 存储新添加的游戏对象
        void CollectRenderComponents(std::vector<RenderComponent *> &renderComponents);

        void SortRenderComponents(std::vector<RenderComponent *> &renderComponents);

    private:
        std::string m_name; // 场景名称
        std::string m_path; // 场景文件路径

        public:

        rapidjson::Value Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const override;

        void Deserialize(const rapidjson::Value &jsonValue) override;

        const std::string &GetName() const;

        const std::string &GetPath() const;

        void SetName(const std::string &name);

        void SetPath(const std::string &path);
    };
} // namespace framework
