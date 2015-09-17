#pragma once

#include "../../ecs/Traits/Traits.hpp"

namespace ecs
{
	namespace Impl
	{	
		// A system is an optional syntactic-sugar abstraction over signature entity matching.
		template<typename TManager, template<typename> class TDerived, typename TSignature>
		class System : Traits::SystemTag
		{
			static_assert(Traits::IsSignature<TSignature>{}, "");

			public:
				using Signature = TSignature;
				TManager* manager;

				auto& getManager() noexcept
				{
					return *manager;
				}

				template<typename TFunction>
				void forEntitiesMatching(TFunction&& mFunction)
				{
					getManager().template forEntitiesMatching<TDerived>(mFunction);
				}
		};
	}
}