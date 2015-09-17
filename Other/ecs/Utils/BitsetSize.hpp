#pragma once

#include <bitset>

namespace ecs
{
	template<typename T> 
	using BitsetSize = std::integral_constant<std::size_t, T{}.size()>;
}