#pragma once

#include <vector>
#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"

namespace ecs
{
	namespace Impl
	{
		namespace Storage
		{
			template<typename... TComponents>
			class Vector final : Traits::StorageTag
			{
				template<typename...> friend class ::ecs::Impl::ComponentConfig;

				private:
					using ComponentTuple = std::tuple<TComponents...>;
					using ComponentTypeList = MPL::TypeList<TComponents...>;
					using Storage = GrowableArray<ComponentTuple>;
					Storage storage;

				public:
					template<typename TComponentsTypeList>
					static constexpr bool storesAll() noexcept
					{
						return MPL::ContainsAll
						<
							TComponentsTypeList,
							ComponentTypeList
						>{};
					}

					auto& getComponentTupleByDataIndex(DataIndex mX) noexcept
					{
						return storage[mX];
					}

					void grow(std::size_t mOldCapacity, std::size_t mNewCapacity)
					{
						storage.grow(mOldCapacity, mNewCapacity);
					}

					void swapAt(std::size_t mA, std::size_t mB) noexcept
					{
						std::swap(storage[mA], storage[mB]);
					}
			};
		}
	}
}