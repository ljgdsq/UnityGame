#pragma once
#include "Framework/Asset/Asset.h"
#include "imgui.h"
namespace editor
{
    class AssetInspector
    {
    public:
        AssetInspector() = default;
        virtual ~AssetInspector() = default;
        virtual void Update(float deltaTime) {}

    public:
        virtual void Inspect(const std::shared_ptr<framework::Asset> &asset);
        virtual void OnSaveClick(const std::shared_ptr<framework::Asset> &asset);

    private:
        bool isEditable;
    };
}
