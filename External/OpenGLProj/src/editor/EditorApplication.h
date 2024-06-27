//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_EDITORAPPLICATION_H
#define OPENGLPROJ_EDITORAPPLICATION_H


#include "core/Application.h"
#include "EditorScene.h"
#include "ImGuiLogSink.h"

class EditorApplication: public Application {
private:
    EditorScene* editorScene= nullptr;
    FrameBuffer *frameBuffer= nullptr;
    static EditorApplication*Instance;
    std::shared_ptr<ImGuiLogSink> imGuiLogSink;
protected:
    void Init() override;

    void OnExit() override;

    void BeginFrame() override;

    void EndFrame() override;
public:
    ~EditorApplication() override;

public:
    EditorApplication(int width,int height,const char*title= nullptr);

    void SetScene(Scene *scene) override;
    EditorApplication*GetInstance();
};


#endif //OPENGLPROJ_EDITORAPPLICATION_H
