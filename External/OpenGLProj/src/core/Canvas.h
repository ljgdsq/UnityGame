//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_CANVAS_H
#define OPENGLPROJ_CANVAS_H
#include <vector>
#include <queue>
#include "Component.h"
#include "buffer.h"
#include "Context.h"

enum CanvasDrawType{
    Line=1,
    Quad=2,
};

class CanvasDrawCommand{
public:
    virtual CanvasDrawType GetType() const{
        return Quad;
    };
};

class QuadDrawCommand:public CanvasDrawCommand{
public:
    vec3 position;
    vec3 scale;
    vec3 rotate;
    vec2 size;
    int texID;
    int shaderID;

    CanvasDrawType GetType() const override;
};
class CanvasItem:public Component{
public:
    virtual void Process(const Context&ctx)=0;

    explicit CanvasItem(GameObject *gameObject);
};
struct Vertex {
    vec3 position;
    vec2 texCoords;

    Vertex(const vec3 &position, const vec2 &texCoords) : position(position), texCoords(texCoords) {}

    Vertex(float x, float y, float z, float u, float v) : position(x, y, z), texCoords(u, v) {}

};

struct Batch{
    int shaderID;
    int texID;
    std::vector<Vertex> vertices; // 顶点数据
    std::vector<unsigned int> indices; // 索引数据
};

class Canvas: public CanvasItem{
public:
    Canvas();

    explicit Canvas(GameObject *gameObject);

public:
    void Process(const Context&ctx) override;
    void Draw();
    int batchCount=0;
    void OnCreate() override;
protected:
    VertexArray* vao;
    Buffer *vbo;
    Buffer* ebo;

    Batch quadBatch;
    Batch otherBatch;
    void FlushQuad();
    void FlushOther();
    std::string GetType() override;



    void OnEnable() override;

    void OnDisable() override;

    void OnDestroy() override;

public:

    static std::vector<CanvasItem*> childs;
    static std::queue<CanvasDrawCommand*> rendererQueue;
    static void AddCanvasItem(CanvasItem*item);
    static void RemoveCanvasItem(CanvasItem*item);
    static void SubmitDrawCommand(CanvasDrawCommand* command);
private:

    bool isSameBatch(QuadDrawCommand* command);
};




#endif //OPENGLPROJ_CANVAS_H
