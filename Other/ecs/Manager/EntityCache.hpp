#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"
#include "../../ecs/Config/Config.hpp"
#include "../../ecs/Manager/SignatureBitsets.hpp"
#include "../../ecs/Manager/ManagerImpl.hpp"

namespace ecs
{
    namespace Impl
    {
        // TODO:
        template <typename TSettings>
        class Manager<TSettings>::EntityCache final
        {
        private:
            struct CacheImpl
            {
                GrowableArray<EntityIndex> v;
                std::size_t size{0};
            };

            using ComponentConfig = typename TSettings::ComponentConfig;
            using SignatureConfig = typename TSettings::SignatureConfig;
            using SystemConfig = typename TSettings::SystemConfig;
            using MyManager = Manager<TSettings>;

            using CacheType = CacheImpl;
            using SignatureCache =
                typename SignatureConfig::template CacheTuple<CacheType>;
            SignatureCache signatureCache;

            template <typename TSignature>
            static constexpr auto getSignatureTypeID() noexcept
            {
                return SignatureConfig::template getSignatureTypeID<
                    TSignature>();
            }

            template <typename TFunction>
            void forCaches(TFunction&& mFunction)
            {
                Utils::forTuple(
                    [&mFunction](auto& mX)
                    {
                        mFunction(mX);
                    },
                    signatureCache);
            }

            template <typename TSignature>
            auto& getCache() noexcept
            {
                return std::get<getSignatureTypeID<TSignature>()>(
                    signatureCache);
            }

        public:
            void grow(std::size_t mOldCapacity, std::size_t mNewCapacity)
            {
                forCaches([this, mOldCapacity, mNewCapacity](auto& mCache)
                    {
                        mCache.v.grow(mOldCapacity, mNewCapacity);
                    });
            }

            void clear() noexcept
            {
                forCaches([this](auto& mCache)
                    {
                        mCache.size = 0;
                    });
            }

            template <typename TSignature>
            void add(EntityIndex mX)
            {
                auto& c(getCache<TSignature>());
                c.v[c.size++] = mX;
            }

            template <typename TSignature, typename TFunction>
            void forCache(TFunction&& mFunction)
            {
                auto& c(getCache<TSignature>());
                auto iEnd(&(c.v[c.size]));

                for(auto iBegin(&(c.v[0])); iBegin != iEnd; ++iBegin)
                    mFunction(*iBegin);
            }
        };
    }
}