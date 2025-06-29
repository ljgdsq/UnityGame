#include "Framework/Render/Buffer.h"
#include "Buffer.h"
Buffer::Buffer(BufferType type)
{
    this->type = type;
    valid = false;
    GenBuffer();
}

Buffer::~Buffer()
{
    Destroy();
}

void Buffer::GenBuffer()
{
    if (valid)
        return;
    switch (type)
    {
    case BufferType::VAO:
        glGenVertexArrays(1, &id);
        break;
    case BufferType::VBO:
    case BufferType::EBO:
        glGenBuffers(1, &id);
        break;
    case BufferType::FBO:
        glGenFramebuffers(1, &id);
        break;
    }
    valid = true;
}
void Buffer::BindBuffer()
{
    if (!valid)
        return;
    switch (type)
    {
    case BufferType::VAO:
        glBindVertexArray(id);
        break;
    case BufferType::VBO:
        glBindBuffer(GL_ARRAY_BUFFER, id);
        break;
    case BufferType::EBO:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        break;
    case BufferType::FBO:
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        break;
    }
}

void Buffer::UnbindBuffer()
{
    if (!valid)
        return;
    switch (type)
    {
    case BufferType::VAO:
        glBindVertexArray(0);
        break;
    case BufferType::VBO:
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        break;
    case BufferType::EBO:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        break;
    case BufferType::FBO:
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        break;
    }
}

void Buffer::Destroy()
{
    if (!valid)
        return;
    switch (type)
    {
    case BufferType::VAO:
        glDeleteVertexArrays(1, &id);
        break;
    case BufferType::VBO:
        glDeleteBuffers(1, &id);
        break;
    case BufferType::EBO:
        glDeleteBuffers(1, &id);
        break;
    case BufferType::FBO:
        glDeleteFramebuffers(1, &id);
        break;
    }
    valid = false;
}

void Buffer::UpdateData(const void *data, size_t size, BufferUsage usage)
{
    if (!valid)
        return;
    BindBuffer();
    switch (type)
    {
    case BufferType::VBO:
        glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
        break;
    case BufferType::EBO:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
        break;
    default:
        // VAO and FBO do not use UpdateData
        break;
    }
}

VertexArray::VertexArray() : Buffer(BufferType::VAO)
{
}

VertexArray::~VertexArray()
{
}

void VertexArray::AddLayout(BufferLayout layout)
{
    if (layout.index >= layouts.size())
    {
        layouts.resize(layout.index + 10);
    }
    layouts[layout.index] = layout;
}

void VertexArray::EnableAttributes(int startIndex)
{
    if (startIndex < layouts.size())
    {
        glEnableVertexAttribArray(startIndex);
        const auto &layout = layouts[startIndex];
        glVertexAttribPointer(layout.index, layout.nComp, layout.type, layout.normalize ? GL_TRUE : GL_FALSE, layout.stride, reinterpret_cast<const void *>(layout.offset));
    }
}
