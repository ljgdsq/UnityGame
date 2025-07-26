#include "Framework/Editor/Inspector/AssetInspector.h"

void editor::AssetInspector::Inspect(const std::shared_ptr<framework::Asset> &asset)
{
    if (!asset)
        return;

    ImGui::Text("Name : %s", asset->GetName().c_str());
    ImGui::Text("Type : %s", AssetTypeToString(asset->GetType()).c_str());
    ImGui::Text("Size : %d bytes", asset->GetSize());
    ImGui::Text("File Path : %s", asset->GetFilePath().c_str());

}
