//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_LOGWINDOW_H
#define OPENGLPROJ_LOGWINDOW_H

#include "EditorWidget.h"
#include "ImGuiLogSink.h"

class SceneNode;
class LogWindow: public EditorWidget{
public:
    LogWindow(const string &name, EditorScene *scene);

    void Draw() override;
private:
    ImGuiLogSink* logSink= nullptr;
public:
    void setLogSink(ImGuiLogSink *logSink);
    void clear();
    ImGuiLogSink *getLogSink() const;
};


#endif //OPENGLPROJ_LOGWINDOW_H
