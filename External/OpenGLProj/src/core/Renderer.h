//
// Created by zhengshulin on 2024/6/17.
//

#ifndef OPENGLPROJ_RENDERER_H
#define OPENGLPROJ_RENDERER_H

#include "Context.h"

enum ClearMode{
    COLOR_BIT=1,
    DEPTH_BIT=2,
    STENCIL_BIT=4,

};


enum FuncType{
    Depth_Test=1,
};

class Renderer {
public:

    void ClearColor(float r,float g,float b,float a);
    void Clear();
    void SetClearMode(int mode);
    void Present();
    void Enable(FuncType type);
    void Disable(FuncType type);
    Renderer(Context*context);

private:
    unsigned int clearBit;
    float r=0.1f;
    float g=0.1f;
    float b=0.1f;
    float a=1.0f;
    Context*context;
};


#endif //OPENGLPROJ_RENDERER_H
