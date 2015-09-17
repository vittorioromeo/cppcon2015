#pragma once

#include <bitset>
#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"
#include "../../ecs/Config/Config.hpp"

namespace ecs
{
	namespace Impl
	{
		enum class CacheSettings
		{ 
			UseCache,
			DontUseCache
		};

 
		template
		<
			typename TComponentConfig, 
			typename TSignatureConfig, 
			typename TSystemConfig = SystemConfig<>, 
			typename TTagConfig = TagConfig<>,
			CacheSettings TCacheSettings = CacheSettings::DontUseCache
		>
		struct Settings final : Traits::SettingsTag
		{
			static_assert(Traits::IsComponentConfig<TComponentConfig>{}, "");
			static_assert(Traits::IsSignatureConfig<TSignatureConfig>{}, "");
			static_assert(Traits::IsSystemConfig<TSystemConfig>{}, "");
			static_assert(Traits::IsTagConfig<TTagConfig>{}, "");

			using ComponentConfig = TComponentConfig;
			using SignatureConfig = TSignatureConfig;
			using SystemConfig = TSystemConfig;
			using TagConfig = TTagConfig;

			static constexpr bool useCache{TCacheSettings == CacheSettings::UseCache};

			static constexpr std::size_t componentCount{ComponentConfig::componentCount};
			static constexpr std::size_t tagCount{TagConfig::tagCount};
			static constexpr std::size_t bitCount{componentCount + tagCount};

			// TODO: constexpr bitset for signatures
			using EntityBitset = std::bitset<bitCount>;

			static constexpr float growMultiplier{2.f};
			static constexpr std::size_t growAmount{5};

			template<typename TComponent>
			static constexpr auto getComponentTypeID() noexcept
			{				
				return ComponentConfig::template getComponentTypeID<TComponent>();
			}

			/*template<typename TSignature>
			static constexpr auto getSignatureTypeID() noexcept
			{
				static_assert(Traits::IsSignature<TSignature>{}, "");
				return SignatureConfig::template getSignatureTypeID<TSignature>();
			}*/

			template<template<typename> class TSystem>
			static constexpr auto getSystemTypeID() noexcept
			{				
				return SystemConfig::template getSystemTypeID<TSystem>();
			}

			template<typename TTag>
			static constexpr auto getTagTypeID() noexcept
			{				
				return TagConfig::template getTagTypeID<TTag>();
			}

			template<typename TComponent>
			static constexpr auto getComponentBit() noexcept
			{
				return getComponentTypeID<TComponent>();
			}

			template<typename TTag>
			static constexpr auto getTagBit() noexcept
			{
				return componentCount + getTagTypeID<TTag>();
			}
		};
	}
}