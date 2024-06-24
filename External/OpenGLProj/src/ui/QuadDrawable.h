//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_QUADDRAWABLE_H
#define OPENGLPROJ_QUADDRAWABLE_H


#include "core/Drawable.h"

class QuadDrawable: public Drawable{
protected:
    VertexArray *VAO= nullptr;
    Buffer *VBO= nullptr;
public:
    QuadDrawable( Shader*shader);
    ~QuadDrawable();
    void Draw() override;
};


#endif //OPENGLPROJ_QUADDRAWABLE_H
