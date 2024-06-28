//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_TEXTURE_H
#define OPENGLPROJ_TEXTURE_H

#include "glad/gl.h"
#include <string>
enum TextureParam{
    CLAMP_TO_EDGE=GL_CLAMP_TO_EDGE,
    REPEAT=GL_REPEAT
};

class Texture2D {
public:
    int id=-1;
    int width;
    int height;

    int internalFormat;
    int imageFormat;
    int wrapS;
    int wrapT;
    int filterMin;
    int filterMax;
    std::string path;
    Texture2D();
    void Generate(int width,int height,int channel,unsigned char*data);
    void Bind(int slot=0)const;
    void SetWrapMode(TextureParam param);
};


#endif //OPENGLPROJ_TEXTURE_H
