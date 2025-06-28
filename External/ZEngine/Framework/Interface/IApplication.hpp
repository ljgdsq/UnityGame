#ifndef FRAMEWORK_INTERFACE_IAPPLICATION_HPP
#define FRAMEWORK_INTERFACE_IAPPLICATION_HPP

#include "IRuntimeModule.hpp"
#include "Interface.hpp"

namespace framework
{
    namespace interface
    {

        Interface IApplication : Implements IRuntimeModule{
            public :
                virtual ~IApplication() = default;
                virtual void Run() = 0;
                virtual bool ShouldExit() = 0;
                virtual const char* GetName() const = 0;
                virtual void Initialize() = 0;
                virtual void Shutdown() = 0;
                virtual void Update(float deltaTime) = 0;
        };

    } // namespace interface
} // namespace framework

#endif // FRAMEWORK_INTERFACE_IAPPLICATION_HPP