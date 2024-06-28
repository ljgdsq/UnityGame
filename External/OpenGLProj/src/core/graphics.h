//
// Created by zhengshulin on 2024/6/12.
//

#ifndef OPENGLPROJ_GRAPHICS_H
#define OPENGLPROJ_GRAPHICS_H
#include "glad/gl.h"
enum DrawMode{
    LINE=GL_LINE,
    FILL=GL_FILL
};

enum CullFace{
    FRONT=GL_FRONT,
    BACK=GL_BACK,
    BOTH=GL_FRONT_AND_BACK
};

class Graphics {
    Graphics()=default;
public:
    static Graphics*GetInstance();
    void SetDrawMode(DrawMode mode);
    void SetCull(CullFace cullFace);
    void DisableCull();
    void SetFrontFace(bool ccw= true);
};


#endif //OPENGLPROJ_GRAPHICS_H
