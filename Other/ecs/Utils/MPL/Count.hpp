#pragma once

#include <type_traits>
#include "./TypeList.hpp"

namespace ecs
{
	namespace MPL
	{
		namespace Impl
		{
			// Count base case: 0.
			template<typename T, typename TTypeList>
			struct CountHelper : std::integral_constant<std::size_t, 0> 
			{ 

			};

			// Interface type alias.
			template<typename T, typename TTypeList>
			using Count = CountHelper<T, TTypeList>;

			// Count recursive case.
			template<typename T, typename T0, typename... Ts>
			struct CountHelper
			<
				T,
				TypeList<T0, Ts...>
			> : std::integral_constant
			<
				std::size_t,
				(std::is_same<T, T0>{} ? 1 : 0) + Count<T, TypeList<Ts...>>{}
			>
			{

			};

			// Alias for `Count > 0`.
			template<typename T, typename TTypeList>
			using Contains = std::integral_constant<bool, (Count<T, TTypeList>{} > 0)>;
		}
	}
}