//
// Created by zhengshulin on 2024/6/12.
//

#include <iostream>
#include "buffer.h"

void Buffer::genBuffer() {
    if (valid)return;
    switch (type) {
        case VAO:
            glGenVertexArrays(1,&id);
            break;
        case VBO:
        case EBO:
            glGenBuffers(1,&id);
            break;
        case FBO:
            glGenFramebuffers(1,&id);
            break;
    }
    valid= true;
}

unsigned int Buffer::getId() const {
    return id;
}

BufferType Buffer::getType() const {
    return type;
}

void Buffer::destroy() {
    if (!valid)return;
    valid= false;
    switch (type) {
        case VAO:
            unbind();
            glDeleteVertexArrays(1,&id);
            break;
        case VBO:
        case EBO:
            unbind();
            glDeleteBuffers(1,&id);
            break;
        case FBO:
            glDeleteFramebuffers(1,&id);
            break;
    }

}

void Buffer::bind() {
    if (!valid) return;
    switch (type) {
        case VAO:
            glBindVertexArray(id);
            break;
        case VBO:
            glBindBuffer(GL_ARRAY_BUFFER, id);
            break;
        case EBO:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            break;
        case FBO:
            glBindFramebuffer(GL_FRAMEBUFFER,id);
    }
}

void Buffer::unbind() {
    if (!valid) return;
    switch (type) {
        case VAO:
            glBindVertexArray(0);
            break;
        case VBO:
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            break;
        case EBO:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            break;
        case FBO:
            glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

}

void Buffer::uploadData(int size, void *data, DataUsage usage) {
    switch (type) {
        case VBO:
            bind();
            glBufferData(GL_ARRAY_BUFFER,size,data,usage);
            break;
        case EBO:
            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
            break;
    }
}

VertexArray::VertexArray() : Buffer(BufferType::VAO) {

}

void VertexArray::enableAttribute(int n) {
    if (n<layouts.size()){
        glEnableVertexAttribArray(n);
    }
}

void VertexArray::addBufferLayout(BufferLayout layout) {
    if (layout.index>=layouts.size()){
        layouts.resize(layout.index + 10);
    }
    layouts[layout.index]=layout;
    glVertexAttribPointer(layout.index,layout.nComp,layout.type,layout.normalize,layout.stride,(void*)layout.offset);

}


FrameBuffer::FrameBuffer(int w,int h) : Buffer(BufferType::FBO) {
    width=w;
    height=h;
    Init();
}

void FrameBuffer::Init() {
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

    // Create and attach a renderbuffer for depth and stencil attachment (if needed)
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "Framebuffer is not complete!" << std::endl;
        valid= false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::getColorBuffer() const {
    return colorBuffer;
}

int FrameBuffer::getWidth() const {
    return width;
}

int FrameBuffer::getHeight() const {
    return height;
}

void FrameBuffer::destroy() {
    Buffer::destroy();
    glDeleteTextures(1,&colorBuffer);
    glDeleteRenderbuffers(1,&rbo);
}
