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
		template<typename TTag>
		auto Manager<TSettings>::hasTag(EntityIndex mX) const noexcept
		{
			assert(isAlive(mX));
			return getEntity(mX).entityBitset[getTagBit<TTag>()];
		}

		template<typename TSettings>
		template<typename TTag>
		auto Manager<TSettings>::hasTag(const SlimHandle& mX) const noexcept
		{
			return hasTag<TTag>(getEntityIndex(mX));
		}



		template<typename TSettings>
		template<typename TTag>
		void Manager<TSettings>::addTag(EntityIndex mX) noexcept
		{			
			assert(isAlive(mX));
			assert(!hasTag<TTag>(mX));

			getEntity(mX).entityBitset[getTagBit<TTag>()] = true;
		}

		template<typename TSettings>
		template<typename TTag>
		void Manager<TSettings>::addTag(const SlimHandle& mX) noexcept
		{
			addTag<TTag>(getEntityIndex(mX));
		}



		template<typename TSettings>
		template<typename TTag>
		void Manager<TSettings>::removeTag(EntityIndex mX) noexcept
		{
			assert(isAlive(mX));
			assert(!hasTag<TTag>(mX));

			getEntity(mX).entityBitset[getTagBit<TTag>()] = false;
		}

		template<typename TSettings>
		template<typename TTag>
		void Manager<TSettings>::removeTag(const SlimHandle& mX) noexcept
		{
			removeTag<TTag>(getEntityIndex(mX));
		}
	}
}