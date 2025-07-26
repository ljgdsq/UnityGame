#include "Framework/Editor/Inspector/AssetInspector.h"
#include "Framework/Core/EngineFileIO.h"
#include "Framework/Util/JsonUtil.hpp"
#include "Framework/Log/Logger.h"
namespace editor
{
    void AssetInspector::Inspect(const std::shared_ptr<framework::Asset> &asset)
    {
        if (!asset)
            return;

        ImGui::Text("Name : %s", asset->GetName().c_str());
        ImGui::Text("Type : %s", AssetTypeToString(asset->GetType()).c_str());
        ImGui::Text("Size : %d bytes", asset->GetSize());
        ImGui::Text("File Path : %s", asset->GetFilePath().c_str());

        if (ImGui::Button("Save"))
        {
            OnSaveClick(asset);
        }
    }

    void AssetInspector::OnSaveClick(const std::shared_ptr<framework::Asset> &asset)
    {
        if (!asset)
            return;
        rapidjson::Document doc;
        auto json = asset->Serialize(doc.GetAllocator());

        std::string filePath = asset->GetFilePath();
        if (!filePath.empty())
        {
            auto str = framework::JsonUtil::JsonToString(json);
            framework::EngineFileIO::SaveText(filePath, str);
            Logger::Log("Asset saved to %s", filePath.c_str());
        }
        else
        {
            Logger::Warn("Asset file path is empty, cannot save asset.");
        }
    }

} // namespace editor
