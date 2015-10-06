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
            // Map base case: empty list.
            template <template <typename> class TTypeFn, typename>
            struct MapHelper
            {
                using type = TypeList<>;
            };

            // Interface type alias.
            template <template <typename> class TTypeFn, typename TTypeList>
            using Map = typename MapHelper<TTypeFn, TTypeList>::type;

            // Map recursive case: non-empty list.
            template <template <typename> class TTypeFn, typename T,
                typename... Ts>
            struct MapHelper<TTypeFn, TypeList<T, Ts...>>
            {
                using type =
                    Concat<TypeList<TTypeFn<T>>, Map<TTypeFn, TypeList<Ts...>>>;
            };

            static_assert(
                std::is_same<Map<std::add_const_t, TypeList<>>, TypeList<>>{},
                "");

            static_assert(std::is_same<Map<std::add_const_t, TypeList<int>>,
                              TypeList<const int>>{},
                "");

            static_assert(
                std::is_same<Map<std::add_const_t, TypeList<int, float>>,
                    TypeList<const int, const float>>{},
                "");
        }
    }
}