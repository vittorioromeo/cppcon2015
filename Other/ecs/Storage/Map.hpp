#pragma once

#include <map>
#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"

namespace ecs
{
	namespace Impl
	{
		namespace Storage
		{
			template<typename... TComponents>
			class Map final : public Traits::StorageTag
			{
				template<typename...> friend class ::ecs::Impl::ComponentConfig;
				
				private:
					using ComponentTuple = std::tuple<TComponents...>;
					using ComponentTypeList = MPL::TypeList<TComponents...>;
					using Storage = std::map<Impl::DataIndex, ComponentTuple>;
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

					auto& getComponentTupleByDataIndex(DataIndex mX)
					{
						return storage[mX];
					}

					void grow(std::size_t, std::size_t)
					{

					}

					void swapAt(std::size_t mA, std::size_t mB) noexcept
					{
						const auto& temp(storage[mA]);
						storage[mA] = storage[mB];
						storage[mB] = temp;
					}
			};
		}
	}
}