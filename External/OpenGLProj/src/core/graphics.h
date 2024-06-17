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

class Graphics {
    Graphics()=default;
public:
    static Graphics*GetInstance();
    void SetDrawMode(DrawMode mode);
};


#endif //OPENGLPROJ_GRAPHICS_H
