#include "Framework/Editor/Inspector/MaterialAssetInspector.h"
#include "Framework/Asset/ShaderManager.h"
#include "Framework/Editor/AssetEditorDefine.h"
#include "Framework/Asset/ShaderAsset.h"
#include "Framework/Asset/MaterialAsset.h"
namespace editor
{
    REGISTER_ASSET_INSPECTOR(framework::AssetType::Material, MaterialAssetInspector);
    void MaterialAssetInspector::Update(float deltaTime)
    {
        AssetInspector::Update(deltaTime);
    }

    void MaterialAssetInspector::Inspect(const std::shared_ptr<framework::Asset> &asset)
    {
        AssetInspector::Inspect(asset);
        auto shaders = framework::ShaderManager::GetInstance().GetAllShaders();
        // 绘制 下拉列表 显示所有的shader名字
        ImGui::SeparatorText("Select Shader");
        ImGui::Spacing();

        static int selectedShaderIndex = 0;
        if (ImGui::BeginCombo("##ShaderCombo", shaders[selectedShaderIndex]->GetName().c_str()))
        {
            for (int i = 0; i < shaders.size(); ++i)
            {
                const auto &shader = shaders[i];
                bool isSelected = (selectedShaderIndex == i);
                if (ImGui::Selectable(shader->GetName().c_str(), isSelected))
                {
                    selectedShaderIndex = i;
                    // 设置当前材质的着色器
                    auto materialAsset = std::dynamic_pointer_cast<framework::MaterialAsset>(asset);
                    if (materialAsset)
                    {
                        materialAsset->SetShader(shader->GetShader());
                    }
                }
            }
            ImGui::EndCombo();
        }
    }
}
