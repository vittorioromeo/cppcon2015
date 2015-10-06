#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"
#include "../../ecs/Config/Config.hpp"

namespace ecs
{
    namespace Impl
    {
        template <typename TSettings, typename TComponents, typename TTags,
            typename TBitset>
        void initializeSignatureImpl(TBitset& mBitset) noexcept
        {
            MPL::forTypes<TComponents>([&mBitset](auto mX)
                {
                    mBitset[TSettings::template getComponentBit<ECS_TYPE(
                        mX)>()] = true;
                });

            MPL::forTypes<TTags>([&mBitset](auto mX)
                {
                    mBitset[TSettings::template getTagBit<ECS_TYPE(mX)>()] =
                        true;
                });
        }

        template <typename TSettings, typename TSignature, typename TData>
        void initializeSignatureBitsetsImpl(
            TData&, NoForbiddenElements) noexcept
        {
            static_assert(!TSignature::hasForbiddenElements, "");
        }

        template <typename TSettings, typename TSignature, typename TData>
        void initializeSignatureBitsetsImpl(
            TData& mData, HasForbiddenElements) noexcept
        {
            static_assert(TSignature::hasForbiddenElements, "");

            using ForbiddenComponents =
                typename TSignature::ForbiddenComponents;
            using ForbiddenTags = typename TSignature::ForbiddenTags;
            initializeSignatureImpl<TSettings, ForbiddenComponents,
                ForbiddenTags>(mData.forbids);
        }

        template <typename TSettings, typename TSignature, typename TData>
        void initializeSignatureBitsets(TData& mData) noexcept
        {
            // Always initialize required bitsets.
            using RequiredComponents = typename TSignature::RequiredComponents;
            using RequiredTags = typename TSignature::RequiredTags;
            initializeSignatureImpl<TSettings, RequiredComponents,
                RequiredTags>(mData.requires);

            // Initialize forbidden bitset only if they exist.
            initializeSignatureBitsetsImpl<TSettings, TSignature>(
                mData, DispatchForbiddenElements<TSignature>{});
        }
    }
}