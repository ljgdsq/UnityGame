#include "BaseApplication.h"

namespace framework {

    void BaseApplication::Initialize() {
        // Default implementation can be empty or provide basic initialization
    }

    void BaseApplication::Shutdown() {
        // Default implementation can be empty or provide basic shutdown logic
    }

    void BaseApplication::Run() {
        // Default implementation can be empty or provide basic run logic
    }

    void BaseApplication::Update(float deltaTime) {
        // Default implementation can be empty or provide basic update logic
    }

    const char* BaseApplication::GetName() const {
        return "BaseApplication";
    }

    bool BaseApplication::ShouldExit() {
        return false; // Default behavior is to not exit
    }

} // namespace framework
