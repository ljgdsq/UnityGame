//
// Created by zhengshulin on 2024/6/27.
//

#include "Canvas.h"
 std::vector<CanvasItem*> Canvas:: childs;
 std::queue<CanvasDrawCommand> Canvas::rendererQueue;
void Canvas::Process() {
    for (const auto&item:childs){
        if (item->active)
            item->Process();
    }
}

void Canvas::AddCanvasItem(CanvasItem *item) {
    childs.push_back(item);
}

void Canvas::RemoveCanvasItem(CanvasItem *item) {
    auto it=std::find(childs.begin(),childs.end(),item);
    if (it!=childs.end()){
        childs.erase(it);
    }
}

std::string Canvas::GetType() {
    return "Canvas";
}

void Canvas::OnCreate() {

}

void Canvas::OnEnable() {

}

void Canvas::OnDisable() {

}

void Canvas::OnDestroy() {

}

Canvas::Canvas(GameObject *gameObject) : CanvasItem(gameObject) {}

void Canvas::SubmitDrawCommand(CanvasDrawCommand command) {
    rendererQueue.push(command);
}

void Canvas::Draw() {
    if (rendererQueue.empty()) return;
    if (drawCount==0){
        const auto& command=rendererQueue.front();
        rendererQueue.pop();
        if (command.GetType()==Quad){
            static int batch=0;

        }
    }
}

CanvasItem::CanvasItem(GameObject *gameObject) : Component(gameObject) {}

CanvasDrawType QuadDrawCommand::GetType() const {
    return Quad;
}
