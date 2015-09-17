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
		void Manager<TSettings>::growTo(std::size_t mNewCapacity)
		{
			assert(mNewCapacity > capacity);

			componentConfig.grow(capacity, mNewCapacity);
			entities.grow(capacity, mNewCapacity);
			handleData.grow(capacity, mNewCapacity);

			if(TSettings::useCache)
			{
				entityCache.grow(capacity, mNewCapacity);
			}

			for(auto i(capacity); i < mNewCapacity; ++i)
			{
				auto& md(entities[i]);
				auto& hd(handleData[i]);

				// Set metadata values.
				md.dataIndex = i;
				md.entityBitset.reset();
				md.alive = false;
				md.handleDataIndex = i;

				// Set handledata values.
				hd.counter = 0;
				hd.entityIndex = i;
			}

			capacity = mNewCapacity;
		}

		template<typename TSettings>
		void Manager<TSettings>::growBy(std::size_t mX)
		{
			growTo(capacity + mX);
		}

		template<typename TSettings>
		void Manager<TSettings>::growIfNeeded()
		{
			if(ECS_LIKELY(capacity > sizeNext)) return;

			// If there isn't enough memory to store newly created items, grow.
			growTo((capacity + TSettings::growAmount) * TSettings::growMultiplier);
		}
	}
}