//
// Created by zhengshulin on 2024/6/24.
//

#ifndef OPENGLPROJ_TEXTUREINSPECT_H
#define OPENGLPROJ_TEXTUREINSPECT_H

#include "ComponentInspector.h"

class TextureInspect: public ComponentInspector{
public:
    void Inspect(struct SceneNode *node) override;
};

#endif //OPENGLPROJ_TEXTUREINSPECT_H
