//
// Created by zhengshulin on 2024/6/12.
//

#ifndef OPENGLPROJ_BUFFER_H
#define OPENGLPROJ_BUFFER_H
#include "glad/gl.h"
#include <vector>
enum BufferType{
    VAO,
    VBO,
    EBO,
    FBO
};

enum DataUsage{
    STATIC_DRAW=GL_STATIC_DRAW,
    DYNAMIC_DRAW=GL_DYNAMIC_DRAW

};
class Buffer {

public:
    Buffer(BufferType type){
        this->type=type;
        valid= false;
        genBuffer();
        bind();
    }
    void genBuffer();
    void bind();
    void unbind();
    virtual void destroy();
    virtual void uploadData(int size,void*data,DataUsage usage);
private:
    unsigned int id;
    BufferType type;
public:
    unsigned int getId() const;
    BufferType getType() const;
    bool valid;
};

struct BufferLayout{
    int index;
    int nComp;
    int type;
    bool normalize;
    int stride;
    int offset;
};

class VertexArray:public Buffer{
    std::vector<BufferLayout> layouts;
private:
    void uploadData(int size, void *data, DataUsage usage){};
public:
    VertexArray();
    void enableAttribute(int n=0);
    void addBufferLayout(BufferLayout layout);
};

class FrameBuffer:public Buffer{
public:
     FrameBuffer(int w,int h);

private:
    void uploadData(int size, void *data, DataUsage usage){};
    void Init();

private:
    unsigned int colorBuffer;
public:
    unsigned int getColorBuffer() const;

    void destroy() override;

private:
    unsigned int rbo;
    int width;
public:
    int getWidth() const;

    int getHeight() const;

private:
    int height;
};


#endif //OPENGLPROJ_BUFFER_H
