//
// Created by zhengshulin on 2024/6/24.
//

#ifndef OPENGLPROJ_CONTENTBROWSER_H
#define OPENGLPROJ_CONTENTBROWSER_H


#include "EditorWidget.h"
class ContentBrowser: public EditorWidget {

public:
    ContentBrowser(const std::string &name, EditorScene *scene);
    void Draw() override;
    void ListFile();

    void ShowDirectory(const std::string &path);
    std::vector<std::string> currentPath; // 保存当前路径的层次结构
};


#endif //OPENGLPROJ_CONTENTBROWSER_H
