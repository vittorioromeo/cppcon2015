#pragma once

#include <cstddef>
#include "../../ecs/Utils/Utils.hpp"

namespace ecs
{
    namespace Impl
    {
        // Index of "real" component data stored in some structure.
        ECS_STRONG_TYPEDEF(std::size_t, DataIndex);

        // Index of entity metadata, stored in a contiguous structure.
        ECS_STRONG_TYPEDEF(std::size_t, EntityIndex);

        // Index of handle data.
        ECS_STRONG_TYPEDEF(std::size_t, HandleDataIndex);

        // Handle validity counter type.
        ECS_STRONG_TYPEDEF(int, Counter);
    }
}