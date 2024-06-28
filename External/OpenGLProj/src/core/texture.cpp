//
// Created by zhengshulin on 2024/5/21.
//


#include "texture.h"

Texture2D::Texture2D() :
        width(0), height(0), internalFormat(GL_RGBA), imageFormat(GL_RGBA), wrapS(GL_REPEAT), wrapT(GL_REPEAT),
        filterMin(GL_LINEAR), filterMax(GL_LINEAR) {
    glGenTextures(1, (GLuint *) &this->id);
}

void Texture2D::Generate(int width, int height,int channel ,unsigned char *data) {
    this->width = width;
    this->height = height;

    if (channel==4){
        this->internalFormat=GL_RGBA;
        this->imageFormat=GL_RGBA;
    }else if(channel==3){
        this->internalFormat=GL_RGB;
        this->imageFormat=GL_RGB;
    }

    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture2D::Bind(int slot) const
{
    if (slot!=0){
        glActiveTexture(GL_TEXTURE0+slot);
    }
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::SetWrapMode(TextureParam param) {
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
    glBindTexture(GL_TEXTURE_2D, 0);
}

