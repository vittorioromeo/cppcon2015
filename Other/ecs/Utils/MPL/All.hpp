#pragma once

#include <type_traits>
#include "./TypeList.hpp"
#include "./Rename.hpp"

namespace ecs
{
	namespace MPL
	{
		namespace Impl
		{
			// All base case: return true.
			template<template<typename> class, typename...>
			struct All : std::true_type
			{

			};

			// All recursive case: test multiple types.
			template<template<typename> class TTypePred, typename T, typename... Ts>
			struct All
			<
				TTypePred,
				T, Ts...
			> : std::integral_constant<bool, TTypePred<T>{} && Impl::All<TTypePred, Ts...>{}>
			{

			};

			// Bind All's predicate to `TMF`.
			template<template<typename> class TMF>
			struct BoundAll
			{
				template<typename... Ts>
				using type = All<TMF, Ts...>;
			};

			// Unpack a type list into All.
			template<template<typename> class TMF, typename TL>
			using AllTypes = Rename<BoundAll<TMF>::template type, TL>;

			static_assert(All<std::is_const>{}, "");
			static_assert(!All<std::is_const, int>{}, "");
			static_assert(All<std::is_const, const int>{}, "");
			static_assert(AllTypes<std::is_const, TypeList<const int>>{}, "");
		}
	}
}