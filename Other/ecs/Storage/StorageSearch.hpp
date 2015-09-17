#pragma once

#include "../../ecs/Utils/Utils.hpp"

namespace ecs
{
	namespace Impl
	{
		template<typename TComponentsTypeList, typename... TStorages> 
		struct StorageSearch;

		template<typename TComponentsTypeList, typename TCurrentStorage, typename... TRestStorages>
		struct StorageSearch 
		<
			TComponentsTypeList,
			TCurrentStorage, TRestStorages...
		> 
		: std::conditional
		<
			TCurrentStorage::template storesAll<TComponentsTypeList>(),
			TCurrentStorage,
			typename StorageSearch<TComponentsTypeList, TRestStorages...>::type
		>
		{ 

		};

		// TODO: fail instantiation
		template<typename TComponentsTypeList>
		struct StorageSearch<TComponentsTypeList> : std::false_type { };
	}
}