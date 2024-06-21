//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_EDITORAPPLICATION_H
#define OPENGLPROJ_EDITORAPPLICATION_H


#include "core/Application.h"
#include "EditorScene.h"

class EditorApplication: public Application {
private:
    EditorScene* editorScene= nullptr;
protected:
    void Init() override;

    void BeginFrame() override;

    void EndFrame() override;
public:
    ~EditorApplication() override;

public:
    EditorApplication(int width,int height,const char*title= nullptr);

    void SetScene(Scene *scene) override;

};


#endif //OPENGLPROJ_EDITORAPPLICATION_H
