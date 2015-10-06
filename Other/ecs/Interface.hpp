#pragma once

#include "../ecs/Utils/Utils.hpp"
#include "../ecs/Core/Core.hpp"
#include "../ecs/Handle/Handle.hpp"
#include "../ecs/Traits/Traits.hpp"
#include "../ecs/Elements/Elements.hpp"
#include "../ecs/Storage/Storage.hpp"
#include "../ecs/Config/Config.hpp"
#include "../ecs/Manager/Manager.hpp"

namespace ecs
{
    using Impl::Tag;
    using Impl::Component;
    using Impl::System;

    using Impl::IsValidComponent;

    namespace Clause
    {
        using Impl::Clause::Requires;
        using Impl::Clause::Forbids;
    }

    namespace Storage
    {
        using Impl::Storage::Vector;
        using Impl::Storage::Map;
    }

    using Impl::Signature;
    using Impl::SlimHandle;

    using Impl::ComponentConfig;
    using Impl::SignatureConfig;
    using Impl::SystemConfig;
    using Impl::TagConfig;

    using Impl::Settings;
    using Impl::Manager;

    using Impl::EntityIndex;

    using Impl::CacheSettings;
}

#define ECS_COMPONENT(mName) struct mName final : ::ecs::Component<mName>

#define ECS_TAG(mName)                     \
    struct mName final : ::ecs::Tag<mName> \
    {                                      \
    }

#define ECS_SYSTEM(mName, mSignature) \
    template <typename TManager>      \
    struct mName final : ::ecs::System<TManager, mName, mSignature>

// TODO: static entities that will not change their component signature at
// runtime?