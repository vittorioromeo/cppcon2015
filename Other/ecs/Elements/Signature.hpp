#pragma once

#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Clause.hpp"

namespace ecs
{
    namespace Impl
    {
        // Storage for a single bitset.
        template <typename TBitset>
        struct BitsetStorageSingle
        {
            TBitset requires;
        };

        // Storage for a two bitsets.
        template <typename TBitset>
        struct BitsetStorageDouble
        {
            TBitset requires, forbids;
        };

        // Entity-matching signature.
        // An entity may have required components and/or tags.
        // An entity may have forbidden components and/or tags.
        template <typename TRequires,
            typename TForbids = Impl::Clause::Forbids<>>
        struct Signature : Traits::SignatureTag
        {
            // Aliases for required elements.
            using RequiredComponents = typename TRequires::Components;
            using RequiredTags = typename TRequires::Tags;

            // Aliases for forbidden elements.
            using ForbiddenComponents = typename TForbids::Components;
            using ForbiddenTags = typename TForbids::Tags;

            // Shortcuts used in entity matching function for early return.
            static constexpr auto hasForbiddenComponents =
                MPL::size<ForbiddenComponents>() > 0;
            static constexpr auto hasForbiddenTags =
                MPL::size<ForbiddenTags>() > 0;
            static constexpr auto hasForbiddenElements =
                hasForbiddenComponents || hasForbiddenTags;

            // Bitset storage for this signature.
            // One bitset is enough if there are no forbidden elements.
            template <typename TBitset>
            using BitsetStorage = std::conditional_t<!hasForbiddenElements,
                BitsetStorageSingle<TBitset>, BitsetStorageDouble<TBitset>>;
        };
    }
}