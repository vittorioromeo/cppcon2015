#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Manager/ManagerImpl.hpp"

namespace ecs
{
    namespace Impl
    {
        // Auxiliary data structure that redirects handles to the entity data.
        // Also used to check handle validity.
        template <typename TSettings>
        struct Manager<TSettings>::HandleData final
        {
            // Metadata index pointed by this handle data.
            Impl::EntityIndex entityIndex;

            // Handle validity check counter.
            Impl::Counter counter;
        };
    }
}