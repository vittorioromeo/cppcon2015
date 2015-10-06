#pragma once

#include "../../ecs/Traits/Traits.hpp"

namespace ecs
{
    namespace Impl
    {
        // Base tag class. User-defined tags must derive from this class.
        template <typename TDerived>
        class Tag : Traits::TagTag
        {
        };
    }
}