#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"
#include "../../ecs/Config/Config.hpp"
#include "../../ecs/Handle/Handle.hpp"
#include "../../ecs/Manager/SignatureBitsets.hpp"
#include "../../ecs/Manager/ManagerImpl.hpp"

namespace ecs
{
	namespace Impl
	{
		template<typename TSettings>
		void Manager<TSettings>::refresh() noexcept
		{
			if(TSettings::useCache)
			{
				entityCache.clear();
			}

			if(ECS_UNLIKELY(sizeNext == 0))
			{
				size = 0;
				return;
			}

			size = sizeNext = refreshImpl();
		}

		template<typename TSettings>
		auto Manager<TSettings>::refreshImpl() noexcept
		{
			EntityIndex iD{0}, iA{sizeNext - 1};

			while(true)
			{
				// Find dead item from left
				for(; true; ++iD)
				{
					// Order matters.
					if(ECS_UNLIKELY(iD > iA))
					{
						// No more dead items
						return iD;
					}

					if(ECS_UNLIKELY(isDead(iD))) break;
					touchAliveEntity(iD);
				}

				// Find alive item from right
				for(; true; --iA)
				{
					// Order matters.
					if(ECS_UNLIKELY(isAlive(iA))) break;

					touchDeadEntity(iA);

					// New dead entities on the right need to be
					// invalidated. Their handle index doesn't need
					// to be changed.
					invalidateHandle(iA);

					if(ECS_UNLIKELY(iA <= iD))
					{
						// No more alive items
						return iD;
					}

				}

				assert(isAlive(iA));
				assert(isDead(iD));

				std::swap(entities[iA], entities[iD]);

				// Entities must be "touched" after swap.
				touchAliveEntity(iD);
				touchDeadEntity(iA);

				// After swap, the alive entity's handle must be
				// refreshed, but not invalidated.
				refreshHandle(iD);

				// After swap, the dead entity's handle must be
				// both refreshed and invalidated.
				invalidateHandle(iA);
				refreshHandle(iA);

				// Move both iterators
				++iD; --iA;
			}
			
			return iD;
		}
	}
}