#include <iostream>
#include <string>
#include "Framework/Interface/IApplication.hpp"
#include "TestScenes/TestEditorApplication.h"

int main()
{

    framework::TestEditorApplication app;
    app.Initialize();
    app.Run();
    app.Shutdown();

    return 0;
}
