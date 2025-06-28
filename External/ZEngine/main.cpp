#include <iostream>
#include <string>
#include "Framework/Interface/IApplication.hpp"
#include "Framework/Common/BaseApplication.h"


int main() {
    framework::BaseApplication app;
    app.Initialize();
    app.Run();
    app.Shutdown();

    return 0;
}
