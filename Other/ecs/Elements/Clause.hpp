#pragma once

#include "../../ecs/Traits/Traits.hpp"

namespace ecs
{
	namespace Impl
	{
		namespace Clause
		{	
			// Base CTRP class that will allow clause aliasing.
			template<typename TElements, typename... TDeriveds>
			struct ClauseBase : TDeriveds...
			{
				static_assert(MPL::AllTypes<Traits::IsComponentOrTag, TElements>(), "");
				
				using Elements = TElements;
				using Components = MPL::Filter<Traits::IsComponent, Elements>;
				using Tags = MPL::Filter<Traits::IsTag, Elements>;
			};
			
			template<typename... TElements>
			using Requires = ClauseBase<MPL::TypeList<TElements...>, Traits::RequiresTag>;

			template<typename... TElements>
			using Forbids = ClauseBase<MPL::TypeList<TElements...>, Traits::ForbidsTag>;
		}
	}
}