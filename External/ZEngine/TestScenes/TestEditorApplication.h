#pragma once

#include "Framework/Common/BaseApplication.h"

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
};

} // namespace framework
