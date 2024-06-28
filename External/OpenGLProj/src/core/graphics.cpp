//
// Created by zhengshulin on 2024/6/12.
//

#include "graphics.h"

Graphics *Graphics::GetInstance() {
    static Graphics graphics;
    return &graphics;
}

void Graphics::SetDrawMode(DrawMode mode) {
    glPolygonMode(GL_FRONT_AND_BACK,(int)mode);
}

void Graphics::SetCull(CullFace cullFace) {
    glEnable(GL_CULL_FACE);
    glCullFace(cullFace);
}

void Graphics::DisableCull() {
    glDisable(GL_CULL_FACE);
}

void Graphics::SetFrontFace(bool ccw) {
    if (ccw){
        glFrontFace(GL_CCW);
    }else{
        glFrontFace(GL_CW);
    }
}
