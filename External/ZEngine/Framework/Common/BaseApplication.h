#include "Framework/Interface/IApplication.hpp"

namespace framework {
    class BaseApplication : public interface::IApplication {
    public:
        virtual ~BaseApplication() = default;

        // Initialize the application
        void Initialize() override;

        // Shutdown the application
        void Shutdown() override;

        // Run the application
        void Run() override;

        // Update the application with delta time
        void Update(float deltaTime) override;

        // Get the name of the application
        const char* GetName() const override;

        // Check if the application should exit
        bool ShouldExit() override;

    };
} // namespace framework
