#pragma once

#include "../../ecs/Core/Core.hpp"

namespace ecs
{
	namespace Impl
	{
		// "Slim" entity handle.
		// Minimum-overhead handle class that can be passed to manager methods.
		class SlimHandle final
		{
			template<typename> friend class ::ecs::Impl::Manager;

			private:
				HandleDataIndex handleDataIndex;
				Counter counter;
		};
	}
}