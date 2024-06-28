//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_EDITORSCENE_H
#define OPENGLPROJ_EDITORSCENE_H


#include "ui/Scene.h"
#include "ImGuiLogSink.h"
#include <memory>
class Hierarchy;
class Stats;
class EditorWidget;
class Inspector;
class EditorPanel;
class ContentBrowser;
class EditorMenuBar;
class LogWindow;
using std::unique_ptr;
class GameObject;
class EditorScene: public Scene{

private: //todo use smart pointer!
    EditorMenuBar* editorMenuBar;
    Hierarchy*hierarchy;
    Stats*stats;
    Inspector*inspector;
    EditorPanel* editorPanel;
    ContentBrowser*contentBrowser;
    LogWindow*logWindow;
    std::vector<EditorWidget*> editorWidget;
public:
    Scene *getRunningScene() const;
    void InspectNode(SceneNode*node);
    void InspectNode(GameObject*node);
    void SetGameBuffer(void*buffer);
private:
    Scene* runningScene;
public:
    void setRunningScene(Scene *runningScene);
    void setLogSink(ImGuiLogSink*sink);
public:
    EditorScene();
    ~EditorScene();
    void AddChild(EditorWidget*child);
    void OnCreate() override;
    void RenderWidget();
    void Save();
    std::vector<SceneNode *> GetNodes() override;

    std::vector<GameObject*> GetGameObjects();
};


#endif //OPENGLPROJ_EDITORSCENE_H
