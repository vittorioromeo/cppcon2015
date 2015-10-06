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
        template <typename... TComponents>
        struct Manager<TSettings>::MatchCallHelper
        {
            template <typename TFunction>
            static void call(
                EntityIndex mX, ThisType& mManager, TFunction&& mFunction)
            {
                mFunction(
                    mX, mManager.template getComponent<TComponents>(mX)...);
            }
        };

        template <typename TSettings>
        template <typename TSignature, typename TFunction>
        void Manager<TSettings>::expandMatchCall(
            EntityIndex mX, TFunction&& mFunction) noexcept
        {
            static_assert(Traits::IsSignature<TSignature>{}, "");

            using RequiredComponents = typename TSignature::RequiredComponents;
            using Helper = MPL::Rename<MatchCallHelper, RequiredComponents>;

            Helper::call(mX, *this, mFunction);
        }



        template <typename TSettings>
        template <typename TSignature>
        auto Manager<TSettings>::matchesSignatureImpl(
            const EntityBitset& mEntityBitset, NoForbiddenElements) const
            noexcept
        {
            const auto& requiresBitset(getRequiresBitset<TSignature>());

            // Does the entity have all required components?
            if((requiresBitset & mEntityBitset) != requiresBitset) return false;

            return true;
        }

        template <typename TSettings>
        template <typename TSignature>
        auto Manager<TSettings>::matchesSignatureImpl(
            const EntityBitset& mEntityBitset, HasForbiddenElements) const
            noexcept
        {
            if(!matchesSignatureImpl<TSignature>(
                   mEntityBitset, std::false_type{}))
                return false;

            // Does the entity have any forbidden component?
            return (getForbidsBitset<TSignature>() & mEntityBitset).none();
        }

        template <typename TSettings>
        template <typename TSignature>
        auto Manager<TSettings>::matchesSignature(EntityIndex mX) const noexcept
        {
            return matchesSignatureImpl<TSignature>(
                getEntityBitset(mX), DispatchForbiddenElements<TSignature>{});
        }

        template <typename TSettings>
        template <typename TSignature>
        auto Manager<TSettings>::matchesSignature(const SlimHandle& mX) const
            noexcept
        {
            return matchesSignature<TSignature>(getEntityIndex(mX));
        }



        template <typename TSettings>
        template <template <typename> class TSystem>
        auto Manager<TSettings>::matchesSystem(EntityIndex mX) const noexcept
        {
            using SystemType = TSystem<Manager<TSettings>>;
            using SystemSig = typename SystemType::Signature;
            return matchesSignature<SystemSig>(mX);
        }

        template <typename TSettings>
        template <template <typename> class TSystem>
        auto Manager<TSettings>::matchesSystem(const SlimHandle& mX) const
            noexcept
        {
            return matchesSystem<TSystem>(getEntityIndex(mX));
        }
    }
}