#include <iostream>
#include <string>
#include "Framework/Interface/IApplication.hpp"
// #include "TestScenes/TestApplication.h"
#include "TestScenes/TestEditorApplication.h"
#include "TestScenes/AssetSystemTest.h"

int main()
{
    // 运行Asset系统测试
    framework::AssetSystemTest::RunTests();

    framework::TestEditorApplication app;
    app.Initialize();
    app.Run();
    app.Shutdown();

    return 0;
}
