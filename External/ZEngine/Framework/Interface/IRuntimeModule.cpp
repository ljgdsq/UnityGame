#include "Interface.hpp"

namespace framework
{
    namespace interface
    {
        Interface IRuntimeModule
        {
        public:
            virtual ~IRuntimeModule() = default;

            // Initialize the runtime module
            virtual void Initialize() = 0;

            // Shutdown the runtime module
            virtual void Shutdown() = 0;

            // Update the runtime module
            virtual void Update(float deltaTime) = 0;

            // Get the name of the runtime module
            virtual const char *GetName() const = 0;
        };
    }
}