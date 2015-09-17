#pragma once

#include <bitset>
#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"

namespace ecs
{
	namespace Impl
	{
		// Config class keeping track of all signatures.
		template<typename... TSignatures>
		class SignatureConfig final : Traits::SignatureConfigTag
		{
			static_assert(Traits::AreSignature<TSignatures...>{}, "");
			template<typename> friend class ::ecs::Impl::Manager;

			private:
				using AsTypeList = MPL::TypeList<TSignatures...>;

				// Pairs every signature to a specific bitset type, inside of a tuple.
				template<typename TBitset>
				using BitsetPairTuple = std::tuple
				<
					std::pair
					<
						TSignatures, 
						typename TSignatures::template BitsetStorage<TBitset>
					>...
				>;

				// TODO:
				template<typename TCache>
				using CacheTuple = MPL::Rename<std::tuple, MPL::Repeat<MPL::size<AsTypeList>(), TCache>>;
				
				// Returns an unique type ID for a specific signature type.
				template<typename TSignature>
				static constexpr auto getSignatureTypeID() noexcept
				{
					static_assert(Traits::IsSignature<TSignature>{}, "");
					return MPL::IndexOf<TSignature, AsTypeList>::value;
				}
		};

		template<typename TSignature>
		using DispatchForbiddenElements = std::integral_constant<bool, TSignature::hasForbiddenElements>;

		using NoForbiddenElements = std::false_type;
		using HasForbiddenElements = std::true_type;
	}
}