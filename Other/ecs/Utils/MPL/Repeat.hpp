#pragma once

#include <type_traits>
#include "./TypeList.hpp"
#include "./TypeListOps.hpp"

namespace ecs
{
    namespace MPL
    {
        namespace Impl
        {
            // Repeat recursive case: `TTimes` repetitions left.
            template <std::size_t TTimes, typename T>
            struct RepeatHelper
            {
                using type =
                    PushBack<typename RepeatHelper<TTimes - 1, T>::type, T>;
            };

            // Repeat base case: `0` repetitions left.
            template <typename T>
            struct RepeatHelper<0, T>
            {
                using type = TypeList<>;
            };

            // Interface type alias.
            template <std::size_t TTimes, typename T>
            using Repeat = typename RepeatHelper<TTimes, T>::type;
        }
    }
}