//
// Created by zhengshulin on 2024/5/21.
//

#ifndef OPENGLPROJ_TEXTURE_H
#define OPENGLPROJ_TEXTURE_H


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
    void Generate(int width,int height,int channel,unsigned char*data);
    void Bind()const;
};


#endif //OPENGLPROJ_TEXTURE_H
