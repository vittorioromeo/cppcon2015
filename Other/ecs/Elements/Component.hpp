#pragma once

#include "../../ecs/Traits/Traits.hpp"

namespace ecs
{
    namespace Impl
    {
        // Base component class. User-defined components must derive from this
        // class.
        template <typename TDerived>
        class Component : Traits::ComponentTag
        {
        };

        // TODO:
        template <typename T>
        struct IsValidComponent : std::integral_constant<bool, true
                                      // std::is_trivially_copyable<T>{}
                                      // std::is_copy_assignable<T>{} &&
                                      // std::is_copy_constructible<T>{} &&
                                      // std::is_move_assignable<T>{} &&
                                      // std::is_move_constructible<T>{}
                                      >
        {
        };
    }
}