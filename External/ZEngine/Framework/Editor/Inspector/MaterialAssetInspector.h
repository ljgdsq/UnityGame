#pragma once
#include "Framework/Editor/Inspector/AssetInspector.h"
#include "Framework/Graphic/Shader.h"
#include "Framework/Asset/MaterialAsset.h"
namespace editor{
    using Uniform= framework::ShaderReflection::Uniform;
    class MaterialAssetInspector:public AssetInspector{
    public:
        ~MaterialAssetInspector() override = default;

        void Update(float deltaTime) override;

        void Inspect(const std::shared_ptr<framework::Asset> &asset) override;

        void RenderPropertyControl(framework::Material *mt, const Uniform &uniform);
    };
}
