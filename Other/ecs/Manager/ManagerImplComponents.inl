#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"
#include "../../ecs/Config/Config.hpp"
#include "../../ecs/Handle/Handle.hpp"
#include "../../ecs/Manager/SignatureBitsets.hpp"
#include "../../ecs/Manager/ManagerImpl.hpp"

namespace ecs
{
    namespace Impl
    {
        template <typename TSettings>
        template <typename TComponent>
        auto& Manager<TSettings>::getComponent(EntityIndex mX) noexcept
        {
            // assert(isAlive(mX));
            assert(hasComponent<TComponent>(mX));
            return getComponentByDataIndex<TComponent>(getDataIndex(mX));
        }

        template <typename TSettings>
        template <typename TComponent>
        auto& Manager<TSettings>::getComponent(const SlimHandle& mX) noexcept
        {
            return getComponent<TComponent>(getEntityIndex(mX));
        }



        template <typename TSettings>
        template <typename TComponent>
        auto Manager<TSettings>::hasComponent(EntityIndex mX) const noexcept
        {
            // assert(isAlive(mX));
            return getEntity(mX).entityBitset[getComponentBit<TComponent>()];
        }

        template <typename TSettings>
        template <typename TComponent>
        auto Manager<TSettings>::hasComponent(const SlimHandle& mX) const
            noexcept
        {
            return hasComponent<TComponent>(getEntityIndex(mX));
        }



        template <typename TSettings>
        template <typename TComponent, typename... Ts>
        auto& Manager<TSettings>::emplaceComponent(
            EntityIndex mX, Ts&&... mXs) noexcept
        {
            assert(isAlive(mX));
            assert(!hasComponent<TComponent>(mX));

            auto& md(getEntity(mX));
            md.entityBitset[getComponentBit<TComponent>()] = true;

            auto& result(getComponentByDataIndex<TComponent>(md.dataIndex));
            // TODO: call destructor of previous componentt data?
            new(&result) TComponent(ECS_FWD(mXs)...);
            return result;
        }

        template <typename TSettings>
        template <typename TComponent, typename... Ts>
        auto& Manager<TSettings>::emplaceComponent(
            const SlimHandle& mX, Ts&&... mXs) noexcept
        {
            return emplaceComponent<TComponent>(
                getEntityIndex(mX), ECS_FWD(mXs)...);
        }



        template <typename TSettings>
        template <typename TComponent>
        void Manager<TSettings>::removeComponent(EntityIndex mX) noexcept
        {
            assert(isAlive(mX));
            assert(!hasComponent<TComponent>(mX));

            getEntity(mX).entityBitset[getComponentBit<TComponent>()] = false;
        }

        template <typename TSettings>
        template <typename TComponent>
        void Manager<TSettings>::removeComponent(const SlimHandle& mX) noexcept
        {
            removeComponent<TComponent>(getEntityIndex(mX));
        }
    }
}