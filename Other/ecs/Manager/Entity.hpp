#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Manager/ManagerImpl.hpp"

namespace ecs
{
    namespace Impl
    {
        // Entity class.
        // Contains the index of the real component data in the storages.
        // Contains the index of the handledata managing this entity.
        // Contains the bitsets representing components/tags of the entity.
        // Contains the current state (dead or alive) of the entity.
        template <typename TSettings>
        struct Manager<TSettings>::Entity final
        {
            // Index of the components in the component storages.
            DataIndex dataIndex;

            // Index of the handledata pointing to this entity.
            HandleDataIndex handleDataIndex;

            // What components and tags does the entity have?
            EntityBitset entityBitset;

            // Is the entity alive?
            bool alive;
        };
    }
}