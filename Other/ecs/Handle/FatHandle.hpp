#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Handle/SlimHandle.hpp"

namespace ecs
{
	namespace Impl
	{	
		// Handle class that stores a reference to the manager.
		// Abstraction over a slim handle.
		template<typename TManager>
		class FatHandle final
		{
			friend TManager;

			private:
				const SlimHandle slimHandle;
				TManager& manager;

			public:
				FatHandle(TManager& mManager, const SlimHandle& mSlimHandle) noexcept
					: slimHandle{mSlimHandle}, manager{mManager} { }

				template<typename TSignature>
				auto matchesSignature() const noexcept
				{
					return manager.template matchesSignature<TSignature>(slimHandle);
				}

				template<template<typename> class TSystem>
				auto matchesSystem() const noexcept
				{
					return manager.template matchesSystem<TSystem>(slimHandle);
				}

				auto isAlive() const noexcept
				{
					return manager.isAlive(slimHandle);
				}

				auto getEntityIndex() const noexcept
				{
					return manager.getEntityIndex(slimHandle);
				}

				auto isHandleValid() const noexcept
				{
					return manager.isHandleValid(slimHandle);
				}

				auto isDead() const noexcept
				{
					return manager.isDead(slimHandle);
				}	

				auto kill() noexcept
				{
					return manager.kill(slimHandle);
				}

				template<typename TComponent> 
				auto hasComponent() const noexcept
				{
					return manager.template hasComponent<TComponent>(slimHandle);
				}

				template<typename TComponent> 
				auto& getComponent() noexcept
				{
					return manager.template getComponent<TComponent>(slimHandle);
				}

				template<typename TComponent> 
				auto& emplaceComponent() noexcept
				{
					return manager.template emplaceComponent<TComponent>(slimHandle);
				}

				template<typename TComponent> 
				void removeComponent() noexcept
				{
					return manager.template removeComponent<TComponent>(slimHandle);
				}

				template<typename TFunction> 
				void forComponents(TFunction&& mFunction) noexcept
				{
					manager.forComponents(mFunction, slimHandle);
				}

				template<typename TTag> 
				auto hasTag() const noexcept
				{
					return manager.template hasTag<TTag>(slimHandle);
				}
				
				template<typename TTag> 
				void addTag() noexcept
				{
					return manager.template addTag<TTag>(slimHandle);
				}
				
				template<typename TTag> 
				void removeTag() noexcept
				{
					return manager.template removeTag<TTag>(slimHandle);
				}
		};
	}
}