//
// Created by zhengshulin on 2024/5/31.
//

#ifndef OPENGLPROJ_DRAWABLE_H
#define OPENGLPROJ_DRAWABLE_H
#include "core/shader.h"
#include "buffer.h"
#include <functional>
#include "core/Context.h"
class Drawable{
public:
    Shader *shader;
    Drawable(Shader *shader):shader(shader){};
    virtual ~Drawable(){}
public:
    virtual void Draw()=0;
};



#endif //OPENGLPROJ_DRAWABLE_H
