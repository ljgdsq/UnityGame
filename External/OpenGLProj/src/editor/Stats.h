//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_STATS_H
#define OPENGLPROJ_STATS_H


#include "EditorWidget.h"
#include "EditorScene.h"

class Stats: public EditorWidget{
public:
    Stats(const std::string &name, EditorScene *scene);

    void Draw() override;
};


#endif //OPENGLPROJ_STATS_H
