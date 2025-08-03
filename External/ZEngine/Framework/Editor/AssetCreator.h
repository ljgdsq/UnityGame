#pragma once
#include <string>
namespace editor
{
    class AssetCreator
    {
    public:
        static void CreateMaterialAsset(const std::string &name);
        static void CreateEmptyScene(const std::string &name);
        static void CreateTemplateScene(const std::string &name);
    };
} // namespace editor
