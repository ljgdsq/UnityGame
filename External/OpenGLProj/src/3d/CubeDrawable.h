//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_CUBEDRAWABLE_H
#define OPENGLPROJ_CUBEDRAWABLE_H


#include "core/Drawable.h"

class CubeDrawable: public Drawable{
protected:
    VertexArray*VAO= nullptr;
    Buffer*VBO= nullptr;

public:
    CubeDrawable( Shader*shader);
    ~CubeDrawable();

    void Draw() override;

};


#endif //OPENGLPROJ_CUBEDRAWABLE_H
