#pragma once
#include "Framework/Editor/Inspector/AssetInspector.h"

namespace editor{
    class MaterialAssetInspector:public AssetInspector{
    public:
        ~MaterialAssetInspector() override = default;

        void Update(float deltaTime) override;

        void Inspect(const std::shared_ptr<framework::Asset> &asset) override;
    };
}