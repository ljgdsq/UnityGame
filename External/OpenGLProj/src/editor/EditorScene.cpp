//
// Created by zhengshulin on 2024/6/21.
//

#include "EditorScene.h"
#include "EditorWidget.h"
#include "Hierarchy.h"
#include "Stats.h"
#include "Inspector.h"
#include "EditorPanel.h"
#include "ContentBrowser.h"
#include "EditorMenuBar.h"
#include "LogWindow.h"

EditorScene::EditorScene() : hierarchy(nullptr), stats(nullptr) {}

void EditorScene::OnCreate() {
    Scene::OnCreate();

    editorMenuBar = new EditorMenuBar("editorMenuBar", this);
    this->AddChild(editorMenuBar);

    hierarchy = new Hierarchy("hierarchy", this);
    this->AddChild(hierarchy);

    stats = new Stats("stats", this);
    this->AddChild(stats);

    inspector = new Inspector("inspector", this);
    this->AddChild(inspector);

    editorPanel = new EditorPanel("editorPanel", this);
    this->AddChild(editorPanel);

    contentBrowser = new ContentBrowser("contentBrowser", this);
    this->AddChild(contentBrowser);

    logWindow=new LogWindow("logWindow",this);
    this->AddChild(logWindow);

}

EditorScene::~EditorScene() {
    if (hierarchy)
        delete hierarchy;
    if (stats)
        delete stats;

    if (inspector)
        delete inspector;

    if (editorPanel)
        delete editorPanel;
    if (contentBrowser)
        delete contentBrowser;
}

void EditorScene::AddChild(EditorWidget *child) {
    editorWidget.push_back(child);
}


void EditorScene::RenderWidget() {

    for (const auto widget: editorWidget) {
        widget->Draw();
    }
}

void EditorScene::setRunningScene(Scene *runningScene) {
    EditorScene::runningScene = runningScene;
}

Scene *EditorScene::getRunningScene() const {
    return runningScene;
}

vector<SceneNode *> EditorScene::GetNodes() {
    return runningScene->GetNodes();
}

void EditorScene::InspectNode(SceneNode *node) {
    inspector->InspectNode(node);
}

void EditorScene::SetGameBuffer(void *buffer) {
    if (editorPanel) {
        editorPanel->ShowGame(buffer);
    }
}

void EditorScene::Save() {

}

void EditorScene::setLogSink(ImGuiLogSink *sink) {
    if(logWindow)
        logWindow->setLogSink(sink);
}

