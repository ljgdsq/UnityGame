#include <iostream>
#include <string>
#include "Framework/Interface/IApplication.hpp"
#include "Framework/TestScenes/TestApplication.h"

int main() {
    framework::TestApplication app;
    app.Initialize();
    app.Run();
    app.Shutdown();

    return 0;
}
