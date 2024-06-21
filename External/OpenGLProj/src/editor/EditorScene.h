//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_EDITORSCENE_H
#define OPENGLPROJ_EDITORSCENE_H


#include "ui/Scene.h"

class Hierarchy;
class Stats;
class EditorWidget;
class Inspector;

class EditorScene: public Scene{

private:
    Hierarchy*hierarchy;
    Stats*stats;
    Inspector*inspector;
    std::vector<EditorWidget*> editorWidget;
public:
    Scene *getRunningScene() const;
    void InspectNode(SceneNode*node);
private:
    Scene* runningScene;
public:
    void setRunningScene(Scene *runningScene);

public:
    EditorScene();
    ~EditorScene();
    void AddChild(EditorWidget*child);
    void OnCreate() override;
    void RenderWidget();;

    std::vector<SceneNode *> GetNodes() override;
};


#endif //OPENGLPROJ_EDITORSCENE_H
