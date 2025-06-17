#include "IRuntimeModule.hpp"
namespace framework
{
    namespace interface
    {

        Interface IApplication : Implements IRuntimeModule{
            public :
                virtual ~IApplication() = default;
                virtual void Run() = 0;
                virtual bool ShouldExit() = 0;
        };

    } // namespace interface
} // namespace framework