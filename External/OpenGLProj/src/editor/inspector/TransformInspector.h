//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_TRANSFORMINSPECTOR_H
#define OPENGLPROJ_TRANSFORMINSPECTOR_H
#include "ComponentInspector.h"

class TransformInspector: public ComponentInspector{
public:
    void Inspect(class GameObject *node) override;
};


#endif //OPENGLPROJ_TRANSFORMINSPECTOR_H
