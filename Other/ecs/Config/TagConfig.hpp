#pragma once

#include <bitset>
#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"

namespace ecs
{
	namespace Impl
	{
		template<typename... TTags>
		class TagConfig final : Traits::TagConfigTag
		{
			static_assert(Traits::AreTag<TTags...>{}, "");
			static_assert(MPL::Unique<TTags...>{}, "");
			template<typename> friend class ::ecs::Impl::Manager;

			public:
				static constexpr std::size_t tagCount{sizeof...(TTags)};
				using AsTypeList = MPL::TypeList<TTags...>;

				// Returns an unique type ID for a specific tag type.
				template<typename TTag>
				static constexpr auto getTagTypeID() noexcept
				{
					static_assert(Traits::IsTag<TTag>{}, "");
					return MPL::IndexOf<TTag, AsTypeList>::value;
				}
		};
	}
}