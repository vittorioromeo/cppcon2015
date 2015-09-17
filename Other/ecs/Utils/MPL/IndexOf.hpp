#pragma once

#include <type_traits>
#include "./TypeList.hpp"

namespace ecs
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename, typename>
			struct IndexOf;

			// IndexOf base case: found the type we're looking for.
			template<typename T, typename... Ts>
			struct IndexOf<T, Impl::TypeList<T, Ts...>>
				: std::integral_constant<std::size_t, 0>
			{

			};

			// IndexOf recursive case: 1 + IndexOf the rest of the types.
			template<typename T, typename TOther, typename... Ts>
			struct IndexOf<T, Impl::TypeList<TOther, Ts...>>
				: std::integral_constant<std::size_t, 1 + Impl::IndexOf<T, Impl::TypeList<Ts...>>{}>
			{

			};
		}
	}
}