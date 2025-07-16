#pragma once
#include <vector>
#include "glad/glad.h"

enum class BufferType
{
    VAO,
    VBO,
    EBO,
    FBO
};

enum class BufferUsage
{
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,
    StreamDraw = GL_STREAM_DRAW
};

class Buffer
{
public:
    Buffer(BufferType type);
    virtual ~Buffer();
    void BindBuffer();
    void UnbindBuffer();
    virtual void Destroy();

    virtual void UpdateData(const void *data, size_t size, BufferUsage usage = BufferUsage::StaticDraw);

private:
    void GenBuffer();
protected:
    bool valid;
    unsigned int id;
    BufferType type;
};

struct BufferLayout
{
    int index;
    int nComp;
    int type;
    bool normalize;
    int stride;
    int offset;
};

class VertexArray : public Buffer
{
    std::vector<BufferLayout> layouts;

public:
    VertexArray();
    ~VertexArray() override;
    void AddLayout(BufferLayout layout);
    void EnableAttributes(int startIndex = 0);
};

class FrameBuffer : public Buffer
{
private:
    unsigned int colorAttachment;
    unsigned int rbo;
    unsigned int width;
    unsigned int height;

public:
    FrameBuffer(unsigned int width, unsigned int height);
    ~FrameBuffer() override;
    virtual void UpdateData(const void *data, size_t size, BufferUsage usage = BufferUsage::StaticDraw) override;
    unsigned int GetColorBuffer() const;
    int GetWidth() const;

    int GetHeight() const;
    virtual void Destroy() override;

private:
    void GenFrameBuffer();
};