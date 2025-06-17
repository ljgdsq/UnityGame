#pragma once

namespace framework {
namespace interface {

    class IRuntimeModule {
    public:
        virtual ~IRuntimeModule() = default;

        // Initialize the module
        virtual void Initialize() = 0;

        // Shutdown the module
        virtual void Shutdown() = 0;

        // Update the module with delta time
        virtual void Update(float deltaTime) = 0;
    };

} // namespace interface
} // namespace framework
