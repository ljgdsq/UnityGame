#include <iostream>
#include <string>
#include "Framework/Interface/IApplication.hpp"
#include "TestScenes/TestApplication.h"

int main() {
    framework::TestApplication app;
    app.Initialize();
    app.Run();
    app.Shutdown();

    return 0;
}
