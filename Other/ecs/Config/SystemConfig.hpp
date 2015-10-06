#pragma once

#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/System.hpp"

namespace ecs
{
    namespace Impl
    {
        // Wraps a "template template class" in a class that can be used inside
        // type lists.
        template <template <typename> class TSystem>
        struct SystemWrapper final
        {
        };

        template <template <typename> class... TSystems>
        struct SystemConfig final : Traits::SystemConfigTag
        {
        public:
            static constexpr std::size_t systemCount{sizeof...(TSystems)};
            using AsTypeList = MPL::TypeList<SystemWrapper<TSystems>...>;

            // Type list of all system types, specialized with `TManager`.
            template <typename TManager>
            using AsTypeListWithManager = MPL::TypeList<TSystems<TManager>...>;

            // Returns an unique type ID for a specific system type.
            template <template <typename> class TSystem>
            static constexpr auto getSystemTypeID() noexcept
            {
                static_assert(
                    MPL::Contains<SystemWrapper<TSystem>, AsTypeList>{}, "");
                return MPL::IndexOf<SystemWrapper<TSystem>, AsTypeList>::value;
            }
        };
    }
}