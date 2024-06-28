//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_EDITORWIDGET_H
#define OPENGLPROJ_EDITORWIDGET_H

#include <string>
#include <vector>
using std::vector;
using std::string;
class EditorScene;

class EditorWidget{
protected:
    std::string name;
    class EditorScene*scene;
public:
    EditorWidget(std::string name,class EditorScene*scene);

public:
    ~EditorWidget(){}
    virtual void Draw()=0;
};


#endif //OPENGLPROJ_EDITORWIDGET_H
