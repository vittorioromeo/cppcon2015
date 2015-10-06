#pragma once

#include "./Interface.hpp"

namespace ecs
{
    namespace MPL
    {
        static_assert(
            ContainsAll<TypeList<int>, TypeList<int, float, char>>{}, "");

        static_assert(
            ContainsAll<TypeList<int, float>, TypeList<int, float, char>>{},
            "");

        static_assert(ContainsAll<TypeList<int, float, char>,
                          TypeList<int, float, char>>{},
            "");

        static_assert(Count<int, TypeList<int>>{} == 1, "");

        static_assert(Count<int, TypeList<>>{} == 0, "");

        static_assert(Count<int, TypeList<int, float, int>>{} == 2, "");

        static_assert(Unique<>{}, "");
        static_assert(Unique<int>{}, "");
        static_assert(Unique<int, float>{}, "");
        static_assert(!Unique<int, int>{}, "");
        static_assert(!Unique<float, float, int, double>{}, "");
    }
}