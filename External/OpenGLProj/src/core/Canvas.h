//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_CANVAS_H
#define OPENGLPROJ_CANVAS_H
#include <vector>
#include <queue>
#include "Component.h"

enum CanvasDrawType{
    Quad=1
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
    virtual void Process()=0;

    explicit CanvasItem(GameObject *gameObject);
};

class Canvas: public CanvasItem{
public:
    Canvas();

    explicit Canvas(GameObject *gameObject);

public:
    void Process() override;
    void Draw();
protected:
    std::string GetType() override;

    void OnCreate() override;

    void OnEnable() override;

    void OnDisable() override;

    void OnDestroy() override;

public:
    static std::vector<CanvasItem*> childs;
    static std::queue<CanvasDrawCommand> rendererQueue;
    static void AddCanvasItem(CanvasItem*item);
    static void RemoveCanvasItem(CanvasItem*item);
    static void SubmitDrawCommand(CanvasDrawCommand command);
private:

    int drawCount=0;
};




#endif //OPENGLPROJ_CANVAS_H
