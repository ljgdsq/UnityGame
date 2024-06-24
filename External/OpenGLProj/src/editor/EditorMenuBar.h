//
// Created by zhengshulin on 2024/6/24.
//

#ifndef OPENGLPROJ_EDITORMENUBAR_H
#define OPENGLPROJ_EDITORMENUBAR_H

#include "EditorWidget.h"

class EditorMenuBar: public EditorWidget{
public:
    EditorMenuBar(const std::string &name, EditorScene *scene);

    void Draw() override;
};


#endif //OPENGLPROJ_EDITORMENUBAR_H
