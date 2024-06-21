//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_EDITORPANEL_H
#define OPENGLPROJ_EDITORPANEL_H

#include <string>
class EditorPanel {
public:
    std::string name;
    EditorPanel(std::string name){
        this->name=name;
    }
public:
    virtual void Draw();
};


#endif //OPENGLPROJ_EDITORPANEL_H
