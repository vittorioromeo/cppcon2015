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
        template <typename TSettings>
        class Manager<TSettings>::SystemsStorage final
        {
        private:
            using SystemConfig = typename TSettings::SystemConfig;
            using MyManager = Manager<TSettings>;

            class SystemMetadata
            {
            };

            template <typename TSystem>
            using SystemPair = std::pair<TSystem, SystemMetadata>;

            using SystemPairsTuple = MPL::Tuple<MPL::Map<SystemPair,
                typename SystemConfig::template AsTypeListWithManager<
                                                             Manager>>>;

            SystemPairsTuple systemsPairsTuple;


            template <template <typename> class TSystem>
            auto& getSystemPair() noexcept
            {
                return std::get<getSystemTypeID<TSystem>()>(systemsPairsTuple);
            }

        public:
            SystemsStorage(MyManager& mManager) noexcept
            {
                forSystems([&mManager](auto& mSystem, auto&)
                    {
                        mSystem.manager = &mManager;
                    });
            }

            static constexpr std::size_t systemCount{SystemConfig::systemCount};

            template <template <typename> class TSystem>
            auto& getSystem() noexcept
            {
                return std::get<0>(getSystemPair<TSystem>());
            }

            template <template <typename> class TSystem>
            auto& getSystemMetadata() noexcept
            {
                return std::get<1>(getSystemPair<TSystem>());
            }

            template <typename TFunction>
            void forSystems(TFunction&& mFunction)
            {
                Utils::forTuple(
                    [&mFunction](auto& mPair)
                    {
                        mFunction(std::get<0>(mPair), std::get<1>(mPair));
                    },
                    systemsPairsTuple);
            }

            template <typename TFunction>
            void forSystems(TFunction&& mFunction) const
            {
                Utils::forTuple(
                    [&mFunction](auto& mPair)
                    {
                        mFunction(std::get<0>(mPair), std::get<1>(mPair));
                    },
                    systemsPairsTuple);
            }
        };
    }
}