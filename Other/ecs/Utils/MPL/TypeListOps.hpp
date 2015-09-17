#pragma once

#include <type_traits>
#include <tuple>
#include "../MetaFor.hpp"
#include "./Type.hpp"
#include "./TypeList.hpp"
#include "./Rename.hpp"
#include "./Concat.hpp"
#include "./Map.hpp"

namespace ecs
{
	namespace MPL
	{
		namespace Impl
		{
			// Type list as a tuple.
			template<typename TTypeList>
			using Tuple = Rename<std::tuple, TTypeList>;

			// Type list as a tuple of Type<T>.
			template<typename TTypeList>
			using TypeTuple = Tuple<Map<Type, TTypeList>>;

			// "Nth" element of a type list.
			template<std::size_t TIndex, typename TTypeList>
			using Nth = std::tuple_element_t
			<
				TIndex,
				Tuple<TTypeList>
			>;

			// Adds a type to the back of a type list.
			template<typename TTypeList, typename T>
			using PushBack = Concat<TTypeList, TypeList<T>>;

			// Adds a type to the front of a type list.
			template<typename TTypeList, typename T>
			using PushFront = Concat<TypeList<T>, TTypeList>;

			// Size of a type list.
			template<typename TTypeList>
			constexpr auto size() noexcept
			{
				return TTypeList::size;
			}

			// Execute a function for every type in a type list, wrapped in a Type<T>.
			template<typename TTypeList, typename TFunction>
			constexpr void forTypes(TFunction&& mFunction) noexcept
			{
				Utils::forTuple(mFunction, TypeTuple<TTypeList>{});
			}

			// First type of a type list.
			template<typename TTypeList>
			using Head = Nth<0, TTypeList>;

			// Last type of a type list.
			template<typename TTypeList>
			using Tail = Nth<size<TTypeList>() - 1, TTypeList>;
		}
	}
}