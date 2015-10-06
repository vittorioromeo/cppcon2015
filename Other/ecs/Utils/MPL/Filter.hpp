#pragma once

#include <type_traits>
#include "./TypeList.hpp"
#include "./Concat.hpp"

namespace ecs
{
    namespace MPL
    {
        namespace Impl
        {
            // Filter base case: empty list.
            template <template <typename> class TTypePred, typename>
            struct FilterHelper
            {
                using type = TypeList<>;
            };

            // Interface type alias.
            template <template <typename> class TTypePred, typename TTypeList>
            using Filter = typename FilterHelper<TTypePred, TTypeList>::type;

            // Filter recursive case: concat matching types.
            template <template <typename> class TTypePred, typename T,
                typename... Ts>
            struct FilterHelper<TTypePred, TypeList<T, Ts...>>
            {
                using Next = Filter<TTypePred, TypeList<Ts...>>;

                using type = std::conditional_t<TTypePred<T>{},
                    Concat<TypeList<T>, Next>, Next>;
            };
        }
    }
}