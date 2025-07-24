#pragma once

#include "Framework/Asset/Asset.h"
#include "Framework/Core/Scene.h"

namespace framework {
    class SceneAsset : public Asset {
    public:
        SceneAsset(const std::string &name);

        ~SceneAsset() override = default;

        virtual long GetSize() const override;

        void GenerateThumbnail() override;

        std::shared_ptr<Scene> GetScene() const { return m_Scene; }

        void SetScene(std::shared_ptr<Scene> scene) { m_Scene = scene; }

        rapidjson::Value Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const override;

        void Deserialize(const rapidjson::Value &json) override;

    private:
        std::shared_ptr<Scene> m_Scene;
    };
} // namespace framework
