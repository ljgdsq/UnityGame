//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_PLANEQUADDRAWABLE_H
#define OPENGLPROJ_PLANEQUADDRAWABLE_H

#include "core/Drawable.h"

class PlaneQuadDrawable: public Drawable{
protected:
    VertexArray *VAO= nullptr;
    Buffer *VBO= nullptr;
public:
    PlaneQuadDrawable(const Shader&shader);
    ~PlaneQuadDrawable();
    void Draw() override;
};



#endif //OPENGLPROJ_PLANEQUADDRAWABLE_H
