#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Storage/Storage.hpp"
#include "../../ecs/Elements/Elements.hpp"

namespace ecs
{
	namespace Impl
	{
		// Stores all the component types.
		// Assigns an unique ID value to every component type.
		template<typename... TComponents>
		class TypesConfig final : Traits::TypesConfigTag
		{
			static_assert(Traits::AreComponent<TComponents...>{}, "");
			static_assert(MPL::All<IsValidComponent, TComponents...>{}, "");
			template<typename> friend class ::ecs::Impl::Manager;

			public:
				using ComponentTypes = MPL::TypeList<TComponents...>;
				static constexpr std::size_t componentCount{MPL::size<ComponentTypes>()};

				template<typename TComponent>
				static constexpr auto getComponentTypeID() noexcept
				{
					static_assert(Traits::IsComponent<TComponent>{}, "");
					return MPL::IndexOf<TComponent, ComponentTypes>::value;
				}
		};

		// Stores component types and component storage types.
		// Matches components with their storage.
		// Contains helper functions to find storage specific to one or more components.
		template<typename... TStorages>
		class ComponentConfig final : Traits::ComponentConfigTag
		{
			static_assert(Traits::AreStorage<TStorages...>{}, "");
			template<typename> friend class ::ecs::Impl::Manager;

			public:
				using StoragesTuple = std::tuple<TStorages...>;

				// Extract all component types from the passed storage settings.
				// Concatenates the component types in a single MPL list.				
				using ComponentTypes = MPL::Concat<typename TStorages::ComponentTypeList...>;
				static_assert(MPL::Rename<MPL::Unique, ComponentTypes>{}, "");

				// Converts the list of component types to `TypesConfig`.
				using TypesConfig = MPL::Rename<TypesConfig, ComponentTypes>;

				static constexpr std::size_t componentCount{TypesConfig::componentCount};

			private:
				// Finds the storage designed to hold `TComponents`.
				template<typename... TComponents>
				using StorageFor = typename StorageSearch
				<
					MPL::TypeList<TComponents...>, 
					TStorages...
				>::type;

				// Returns true if a suitable storage for `TComponents` exists.
				template<typename... TComponents>
				static constexpr bool hasStorageFor{!std::is_same<StorageFor<TComponents...>, std::false_type>{}};

				// Actual instantiation of the storage types.
				StoragesTuple storages;

				template<typename... TComponents>
				auto& getStorage() noexcept
				{
					static_assert(hasStorageFor<TComponents...>, "");
					return std::get<StorageFor<TComponents...>>(storages);
				}

				template<typename TFunction>
				void forStorages(TFunction&& mFunction)
				{
					Utils::forTuple([&mFunction](auto& mStorage)
					{
						mFunction(mStorage);
					}, storages);
				}

			public:
				template<typename TComponent>
				static constexpr auto getComponentTypeID() noexcept
				{
					return TypesConfig::template getComponentTypeID<TComponent>();
				}

				template<typename TComponent>
				auto& getComponentByDataIndex(DataIndex mX) noexcept
				{
					return std::get<TComponent>
					(
						getComponentTupleByDataIndex<TComponent>(mX)
					);
				}

				template<typename... TComponents>
				auto& getComponentTupleByDataIndex(DataIndex mX) noexcept
				{
					return getStorage<TComponents...>().getComponentTupleByDataIndex(mX);
				}

				void grow(std::size_t mOldCapacity, std::size_t mNewCapacity)
				{
					forStorages([mOldCapacity, mNewCapacity](auto& mX)
					{
						mX.grow(mOldCapacity, mNewCapacity);
					});
				}
		};
	}
}