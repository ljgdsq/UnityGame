#pragma once

#include "Framework/Common/BaseApplication.h"

namespace framework {

class TestApplication : public BaseApplication {
public:
    TestApplication();
    ~TestApplication() override = default;

protected:
    // 重写初始化测试场景的方法
    void InitScenes() override;
    
    // 处理测试相关的按键输入
    void HandleInput() override;
};

} // namespace framework
