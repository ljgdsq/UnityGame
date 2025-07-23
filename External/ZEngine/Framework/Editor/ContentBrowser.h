#pragma once
#include "Framework/Editor/EditorWidget.h"
#include <vector>
#include <string>

namespace editor
{
    class ContentBrowser : public EditorWidget
    {
    public:
        ContentBrowser();
        virtual ~ContentBrowser();

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render() override;
        void Shutdown() override;

    private:
        void RenderFileSystem();
        void RenderAssetDragSource(const std::string &filePath, const std::string &filename, const std::string &extension);

        std::string rootPath;
        std::vector<std::string> currentPath;
    };
}
