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
		template<typename TSignature>
		const auto& Manager<TSettings>::getRequiresBitset() const noexcept
		{
			return bitsetStorage.template getSignatureRequiresBitset<TSignature>();
		}

		template<typename TSettings>
		template<typename TSignature>
		const auto& Manager<TSettings>::getForbidsBitset() const noexcept
		{
			return bitsetStorage.template getSignatureForbidsBitset<TSignature>();
		}

		template<typename TSettings>
		template<typename TComponent>
		auto& Manager<TSettings>::getComponentByDataIndex(DataIndex mX) noexcept
		{
			return componentConfig.template getComponentByDataIndex<TComponent>(mX);
		}




		template<typename TSettings>
		const auto& Manager<TSettings>::getEntity(EntityIndex mX) const noexcept
		{
			assert(sizeNext > mX);
			return entities[mX];
		}

		template<typename TSettings>
		auto& Manager<TSettings>::getEntity(EntityIndex mX) noexcept
		{
			assert(sizeNext > mX);
			return entities[mX];
		}





		template<typename TSettings>
		const auto& Manager<TSettings>::getEntityBitset(EntityIndex mX) const noexcept
		{
			return getEntity(mX).entityBitset;
		}

		template<typename TSettings>
		auto Manager<TSettings>::getDataIndex(EntityIndex mX) const noexcept
		{
			return getEntity(mX).dataIndex;
		}

		template<typename TSettings>
		auto& Manager<TSettings>::getHandleData(HandleDataIndex mX) noexcept
		{
			assert(capacity > mX);
			return handleData[mX];
		}

		template<typename TSettings>
		auto& Manager<TSettings>::getHandleData(EntityIndex mX) noexcept
		{
			return getHandleData(getEntity(mX).handleDataIndex);
		}

		template<typename TSettings>
		auto& Manager<TSettings>::getHandleData(const SlimHandle& mX) noexcept
		{
			return getHandleData(mX.handleDataIndex);
		}

		template<typename TSettings>
		const auto& Manager<TSettings>::getHandleData(HandleDataIndex mX) const noexcept
		{
			assert(capacity > mX);
			return handleData[mX];
		}

		template<typename TSettings>
		const auto& Manager<TSettings>::getHandleData(EntityIndex mX) const noexcept
		{
			return getHandleData(getEntity(mX).handleDataIndex);
		}

		template<typename TSettings>
		const auto& Manager<TSettings>::getHandleData(const SlimHandle& mX) const noexcept
		{
			return getHandleData(mX.handleDataIndex);
		}

		template<typename TSettings>
		Manager<TSettings>::Manager()
		{
			growTo(50);
		}




		template<typename TSettings>
		auto Manager<TSettings>::isHandleValid(const SlimHandle& mX) const noexcept
		{
			return mX.counter == getHandleData(mX).counter;
		}

		template<typename TSettings>
		auto Manager<TSettings>::getEntityIndex(const SlimHandle& mX) const noexcept
		{
			assert(isHandleValid(mX));
			return getHandleData(mX).entityIndex;
		}

		template<typename TSettings>
		auto Manager<TSettings>::getEntityCount() const noexcept
		{
			return size;
		}

		template<typename TSettings>
		auto Manager<TSettings>::getComponentCount() const noexcept
		{
			std::size_t result{0};

			for(EntityIndex i{0}; i < size; ++i)
				result += getEntity(i).entityBitset.count();

			return result;
		}



		template<typename TSettings>
		auto Manager<TSettings>::isAlive(EntityIndex mX) const noexcept
		{
			return getEntity(mX).alive;
		}

		template<typename TSettings>
		auto Manager<TSettings>::isAlive(const SlimHandle& mX) const noexcept
		{
			return isAlive(getEntityIndex(mX));
		}




		template<typename TSettings>
		auto Manager<TSettings>::isDead(EntityIndex mX) const noexcept
		{
			return !isAlive(mX);
		}

		template<typename TSettings>
		auto Manager<TSettings>::isDead(const SlimHandle& mX) const noexcept
		{
			return isDead(getEntityIndex(mX));
		}




		template<typename TSettings>
		auto Manager<TSettings>::kill(EntityIndex mX) noexcept
		{
			getEntity(mX).alive = false;
		}

		template<typename TSettings>
		auto Manager<TSettings>::kill(const SlimHandle& mX) noexcept
		{
			kill(getEntityIndex(mX));
		}




		template<typename TSettings>
		template<typename TCurrent, typename... TRest>
		struct Manager<TSettings>::ComponentFinder
		<
			TCurrent, TRest...
		>
		{
			template<typename TFunction>
			static void call(EntityIndex mX, Manager<TSettings>::ThisType& mManager, TFunction&& mFunction)
			{
				if(mManager.template hasComponent<TCurrent>(mX))
					mFunction(mManager.template getComponent<TCurrent>(mX));

				ComponentFinder<TRest...>::call(mX, mManager, mFunction);
			}
		};

		template<typename TSettings>
		template<typename TFunction>
		void Manager<TSettings>::forComponents(TFunction&& mFunction, EntityIndex mX) noexcept
		{
			assert(isAlive(mX));
			MPL::Rename<ComponentFinder, typename ComponentConfig::ComponentTypes>::call(mX, *this, mFunction);
		}

		template<typename TSettings>
		template<typename TFunction>
		void Manager<TSettings>::forComponents(TFunction&& mFunction, const SlimHandle& mX) noexcept
		{
			forComponents(mFunction, getEntityIndex(mX));
		}


		template<typename TSettings>
		auto Manager<TSettings>::createIndex()
		{
			// Grow (reallocate) if memory is not available.
			
			// TODO: BUG: if entities are created during update,
			// and stuff gets resized, references are invalidated!ù

			// TODO: is this even necessary? Maybe just grow on refresh?
			// ??
			growIfNeeded();

			// Get first available metadata index.
			EntityIndex freeIndex(sizeNext++);		

			// Initialize metadata for the new entity.
			assert(isDead(freeIndex));
			auto& m(entities[freeIndex]);
			
			m.entityBitset.reset();
			m.alive = true;

			return freeIndex;
		}

		template<typename TSettings>
		auto Manager<TSettings>::createSlimHandle()
		{
			auto freeIndex(createIndex());
			assert(isAlive(freeIndex));
			auto& m(entities[freeIndex]);

			auto& hd(handleData[m.handleDataIndex]);
			hd.entityIndex = freeIndex;

			// Initialize a valid entity handle.
			SlimHandle h;
			h.handleDataIndex = m.handleDataIndex;
			h.counter = hd.counter;

			// Assert entity handle validity.
			assert(isHandleValid(h));

			// Return a copy of the entity handle.
			return h;
		}

		template<typename TSettings>
		auto Manager<TSettings>::createFatHandle()
		{
			return FatHandleType{*this, createSlimHandle()};
		}


		template<typename TSettings>
		template<typename TSignature, typename TFunction>
		void Manager<TSettings>::forEntitiesMatchingSignature(TFunction&& mFunction) noexcept
		{
			static_assert(Traits::IsSignature<TSignature>{}, "");

			if(!TSettings::useCache)
			{
				for(EntityIndex i{0u}; i < size; ++i)
					if(matchesSignature<TSignature>(i))
						expandMatchCall<TSignature>(i, mFunction);
			}
			else
			{
				entityCache.template forCache<TSignature>([this, &mFunction](auto mI)
				{
					this->expandMatchCall<TSignature>(mI, mFunction);
				});
			}
		}

		template<typename TSettings>
		template<template<typename> class TSystem, typename TFunction>
		void Manager<TSettings>::forEntitiesMatching(TFunction&& mFunction) noexcept
		{
			using SystemType = TSystem<Manager<TSettings>>;
			static_assert(Traits::IsSystem<SystemType>{}, "");
			using Signature = typename SystemType::Signature;

			forEntitiesMatchingSignature<Signature>(mFunction);
		}

		template<typename TSettings>
		template<template<typename> class TSystem, typename TFunction>
		void Manager<TSettings>::forSystem0(TFunction&& mFunction)
		{
			auto& system(this->getSystem<TSystem>());
			forEntitiesMatching<TSystem>([this, &system, &mFunction](auto&&... xs)
			{
				mFunction(system, ECS_FWD(xs)...);
			});
		}

		template<typename TSettings>
		template<template<typename> class TSystem, typename... Ts, typename... TArgs>
		void Manager<TSettings>::forSystem(void(TSystem<ThisType>::*mFunction)(Ts...), TArgs&&... mArgs)
		{
			auto& system(this->getSystem<TSystem>());
			forEntitiesMatching<TSystem>([this, &system, &mFunction, &mArgs...](auto&&... xs)
			{
				(system.*mFunction)(ECS_FWD(mArgs)..., ECS_FWD(xs)...);
			});
		}




		template<typename TSettings>
		template<template<typename> class TSystem>
		auto& Manager<TSettings>::getSystem() noexcept
		{
			return systemStorage.template getSystem<TSystem>();
		}

		template<typename TSettings>
		auto Manager<TSettings>::getCapacity() const noexcept
		{
			return capacity;
		}
	}
}