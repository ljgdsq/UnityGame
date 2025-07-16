#include "Framework/Render/Buffer.h"
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

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) : Buffer(BufferType::FBO), width(width), height(height)
{
    GenFrameBuffer();
}

FrameBuffer::~FrameBuffer()
{
    Destroy();
}

void FrameBuffer::UpdateData(const void *data, size_t size, BufferUsage usage)
{
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

unsigned int FrameBuffer::GetColorBuffer() const
{
    return colorAttachment;
}

int FrameBuffer::GetWidth() const
{
    return width;
}

int FrameBuffer::GetHeight() const
{
    return height;
}

void FrameBuffer::Destroy()
{
    Buffer::Destroy();
    // glDeleteFramebuffers(1, &id);
    glDeleteTextures(1, &colorAttachment);
    glDeleteRenderbuffers(1, &rbo);
}

void FrameBuffer::GenFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glGenTextures(1, &colorAttachment);
    glBindTexture(GL_TEXTURE_2D, colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

    // Create and attach a renderbuffer for depth and stencil attachment (if needed)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        valid = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        glVertexAttribPointer(layout.index, layout.nComp, layout.type, layout.normalize ? GL_TRUE : GL_FALSE, layout.stride, reinterpret_cast<const void *>(static_cast<size_t>(layout.offset)));
    }
}
