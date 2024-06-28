#ifndef OPENGLPROJ_CONTENTBROWSER_H
#define OPENGLPROJ_CONTENTBROWSER_H


#include "EditorWidget.h"
class ContentBrowser: public EditorWidget {

public:
    ContentBrowser(const std::string &name, EditorScene *scene);
    void Draw() override;
    void ListFile();

    void ShowDirectory(const std::string &path);
    std::vector<std::string> currentPath;
};


#endif //OPENGLPROJ_CONTENTBROWSER_H
