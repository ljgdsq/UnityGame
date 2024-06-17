//
// Created by zhengshulin on 2024/6/12.
//

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

VertexArray::VertexArray(BufferType type) : Buffer(type) {

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
