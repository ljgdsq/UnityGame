#pragma once
#include "Framework/Editor/EditorWidget.h"
#include <vector>
#include <string>

namespace editor{
    class ContentBrowser : public EditorWidget {
    public:
        ContentBrowser();
        virtual ~ContentBrowser();

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render() override;
        void Shutdown() override;

    private:
        void RenderFileSystem();
        void RenderAsset(const std::string& assetPath);
        std::string rootPath;
        std::vector<std::string> currentPath;
    };
}

