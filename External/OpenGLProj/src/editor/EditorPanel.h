//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_EDITORPANEL_H
#define OPENGLPROJ_EDITORPANEL_H
#include "EditorWidget.h"
#include <string>
class EditorPanel :public  EditorWidget{
    FrameBuffer *buffer= nullptr;
public:
    EditorPanel(const std::string name, EditorScene *scene);

    void Draw() override;
    void ShowGame(void *data);
};


#endif //OPENGLPROJ_EDITORPANEL_H
