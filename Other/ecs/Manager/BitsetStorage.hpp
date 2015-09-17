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
		// Manager inner class.
		// Stores bitsets and matches them signatures.
		template<typename TSettings>
		class Manager<TSettings>::BitsetStorage final
		{
			private:
				using ComponentConfig = typename TSettings::ComponentConfig;
				using SignatureConfig = typename TSettings::SignatureConfig;
				using SystemConfig = typename TSettings::SystemConfig;
				using MyManager = Manager<TSettings>;

				using SignatureData = typename SignatureConfig::template BitsetPairTuple<EntityBitset>;
				SignatureData signatureData;

				template<typename TSignature>
				static constexpr auto getSignatureTypeID() noexcept
				{
					return SignatureConfig::template getSignatureTypeID<TSignature>();
				}

				template<typename TSignature>
				auto& getSignatureBitsets() noexcept
				{
					return std::get<getSignatureTypeID<TSignature>()>(signatureData).second;
				}

				template<typename TSignature>
				const auto& getSignatureBitsets() const noexcept
				{
					return std::get<getSignatureTypeID<TSignature>()>(signatureData).second;
				}
 
				
				template<typename TSignature>
				void initBitset() noexcept
				{
					auto& bitsets(getSignatureBitsets<TSignature>());
					initializeSignatureBitsets<Settings, TSignature>(bitsets);
				}

			public:
				BitsetStorage() noexcept
				{
					MPL::forTypes<typename SignatureConfig::AsTypeList>([this](auto mSignature)
					{
						this->initBitset<ECS_TYPE(mSignature)>();
					});
				}

				template<typename TSignature>
				const auto& getSignatureRequiresBitset() const noexcept
				{
					return getSignatureBitsets<TSignature>().requires;
				}

				template<typename TSignature>
				const auto& getSignatureForbidsBitset() const noexcept
				{
					static_assert(TSignature::hasForbiddenElements, "");
					return getSignatureBitsets<TSignature>().forbids;
				}
		};
	}
}