#pragma once

#include <type_traits>
#include "./TypeList.hpp"

namespace ecs
{
    namespace MPL
    {
        namespace Impl
        {
            // Concat base case: empty lists.
            template <typename...>
            struct ConcatHelper
            {
                using type = TypeList<>;
            };

            // Interface type alias.
            template <typename... TTypeLists>
            using Concat = typename ConcatHelper<TTypeLists...>::type;

            // Concat base case: single non-empty list.
            template <typename... Ts>
            struct ConcatHelper<TypeList<Ts...>>
            {
                using type = TypeList<Ts...>;
            };

            // Concat recursive case: multiple lists.
            template <typename... Ts0, typename... Ts1, typename... TRest>
            struct ConcatHelper<TypeList<Ts0...>, TypeList<Ts1...>, TRest...>
            {
                using type = Concat<TypeList<Ts0..., Ts1...>, TRest...>;
            };

            static_assert(std::is_same<Concat<>, TypeList<>>{}, "");

            static_assert(std::is_same<Concat<TypeList<>>, TypeList<>>{}, "");

            static_assert(
                std::is_same<Concat<TypeList<int>>, TypeList<int>>{}, "");

            static_assert(
                std::is_same<
                    Concat<TypeList<int>, TypeList<>, TypeList<int, float>>,
                    TypeList<int, int, float>>{},
                "");
        }
    }
}