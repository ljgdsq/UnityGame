#pragma once
#include "Framework/Editor/Hierarchy.h"
#include "Framework/Editor/GameView.h"
#include "Framework/Editor/ContentBrowser.h"
#include "Framework/Common/BaseApplication.h"
#include "Framework/Editor/Inspector.h"

namespace framework {

class TestEditorApplication : public BaseApplication {
public:
    TestEditorApplication();
    ~TestEditorApplication() override = default;

protected:
    // 重写初始化测试场景的方法
    void InitScenes() override;
    
    // 处理测试相关的按键输入
    void HandleInput() override;

    virtual void BegineFrame() override;
    virtual void EndFrame() override;

    virtual void OnUpdate(float deltaTime) override;

    private:
        editor::GameView* gameView = nullptr; // 游戏视图小部件
        class FrameBuffer * gameFrameBuffer; // 前向声明帧缓冲类
        editor::HierarchyWidget* hierarchyWidget = nullptr; // 层级视图小部件
        editor::Inspector* inspectorWidget = nullptr; // 检查器小部件，用于显示选中游戏对象的属性
        editor::ContentBrowser* contentBrowser = nullptr; // 内容浏览器小部件
    };

} // namespace framework
