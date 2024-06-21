//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_TEXTURE_H
#define OPENGLPROJ_TEXTURE_H

#include "glad/gl.h"

enum TextureParam{
    CLAMP_TO_EDGE=GL_CLAMP_TO_EDGE,
    REPEAT=GL_REPEAT
};

class Texture2D {
public:
    int id;
    int width;
    int height;

    int internalFormat;
    int imageFormat;
    int wrapS;
    int wrapT;
    int filterMin;
    int filterMax;
    Texture2D();
    void load(const char*file);
    void Generate(int width,int height,int channel,unsigned char*data);
    void Bind(int slot=0)const;
    void SetWrapMode(TextureParam param);
};


#endif //OPENGLPROJ_TEXTURE_H
