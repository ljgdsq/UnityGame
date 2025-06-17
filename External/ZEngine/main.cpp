#include <iostream>
#include <string>
#include "Framework/Interface/IApplication.hpp"

class MyApplication : public framework::interface::IApplication {
public:
    void Initialize() override {
        std::cout << "Initializing MyApplication" << std::endl;
    }

    void Shutdown() override {
        std::cout << "Shutting down MyApplication" << std::endl;
    }

    void Run() override {
        std::cout << "Running MyApplication" << std::endl;
    }

    void Update(float deltaTime) override {
        std::cout << "Updating MyApplication with deltaTime: " << deltaTime << std::endl;
    }
    const char* GetName() const override {
        return "MyApplication";
    }
    bool ShouldExit() override {
        return false; // For simplicity, we won't exit in this example
    }
    ~MyApplication() override = default;
};

int main() {
    MyApplication app;
    app.Initialize();
    app.Run();
    app.Update(0.016f);
    app.Shutdown();

    return 0;
}
