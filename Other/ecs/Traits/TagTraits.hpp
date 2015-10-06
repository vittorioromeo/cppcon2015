#pragma once

#include "../../ecs/Utils/Utils.hpp"

// Boilerplate code generation for simple type traits and checkers.
#define ECS_DEFINE_TAG(mName)                                           \
    struct ECS_CAT(mName, Tag)                                          \
    {                                                                   \
    };                                                                  \
                                                                        \
    template <typename T>                                               \
    using ECS_CAT(Is, mName) = std::is_base_of<ECS_CAT(mName, Tag), T>; \
                                                                        \
    template <typename... Ts>                                           \
    using ECS_CAT(Are, mName) = MPL::All<ECS_CAT(Is, mName), Ts...>;

namespace ecs
{
    namespace Impl
    {
        namespace Traits
        {
            ECS_DEFINE_TAG(Requires)
            ECS_DEFINE_TAG(Forbids)
            ECS_DEFINE_TAG(Component)
            ECS_DEFINE_TAG(Storage)
            ECS_DEFINE_TAG(Signature)
            ECS_DEFINE_TAG(System)
            ECS_DEFINE_TAG(Tag)
            ECS_DEFINE_TAG(TypesConfig)
            ECS_DEFINE_TAG(ComponentConfig)
            ECS_DEFINE_TAG(SignatureConfig)
            ECS_DEFINE_TAG(SystemConfig)
            ECS_DEFINE_TAG(TagConfig)
            ECS_DEFINE_TAG(Settings)

            template <typename T>
            struct IsComponentOrTag
                : std::integral_constant<bool, IsComponent<T>{} || IsTag<T>{}>
            {
            };
        }
    }
}

#undef ECS_DEFINE_TAG