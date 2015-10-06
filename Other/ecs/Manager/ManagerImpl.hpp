#pragma once

#include "../../ecs/Core/Core.hpp"
#include "../../ecs/Traits/Traits.hpp"
#include "../../ecs/Elements/Elements.hpp"
#include "../../ecs/Config/Config.hpp"
#include "../../ecs/Handle/Handle.hpp"
#include "../../ecs/Manager/SignatureBitsets.hpp"

namespace ecs
{
    namespace Impl
    {
        template <typename TSettings>
        class Manager final
        {
            static_assert(Traits::IsSettings<TSettings>{}, "");

        private:
            using Settings = TSettings;
            using ThisType = Manager<Settings>;

            using ComponentConfig = typename Settings::ComponentConfig;
            using SignatureConfig = typename Settings::SignatureConfig;
            using SystemConfig = typename Settings::SystemConfig;
            using TagConfig = typename Settings::TagConfig;

            // Assert `SystemConfig` types are actually systems.
            using SystemListWithManager =
                typename SystemConfig::template AsTypeListWithManager<ThisType>;
            static_assert(
                MPL::Rename<Traits::AreSystem, SystemListWithManager>{}, "");

            using EntityBitset = typename Settings::EntityBitset;
            using FatHandleType = FatHandle<ThisType>;

            // Inner classes forward-declarations.
            template <typename... TComponents>
            struct MatchCallHelper;
            template <typename... TComponents>
            struct ComponentFinder;
            class SystemsStorage;
            class BitsetStorage;
            class EntityCache;
            struct Entity;
            struct HandleData;

            // TODO: only instantiate if enabled (or remove)
            EntityCache entityCache;

            // TODO: abstract memory
            // Entity metadata storage. Guarantees contiguous alive entities
            // after refresh.
            GrowableArray<Entity> entities;

            // Handle data storage.
            GrowableArray<HandleData> handleData;

            // Storage capacity (allocated memory).
            std::size_t capacity{0};

            // Current size. Does not take into account newly created entities.
            std::size_t size{0};

            // Next size (after refresh). Takes into account newly created
            // entities.
            std::size_t sizeNext{0};

            // Instantiate config types that require storage.
            ComponentConfig componentConfig;
            SystemsStorage systemStorage{*this};
            BitsetStorage bitsetStorage;

            // Shortcut getter for tag bit.
            template <typename TTag>
            static constexpr auto getTagBit() noexcept
            {
                return Settings::template getTagBit<TTag>();
            }

            // Shortcut getter for component bit.
            template <typename TComponent>
            static constexpr auto getComponentBit() noexcept
            {
                return Settings::template getComponentBit<TComponent>();
            }

            // Shortcut getter for system type id.
            template <template <typename> class TSystem>
            static constexpr auto getSystemTypeID() noexcept
            {
                return Settings::template getSystemTypeID<TSystem>();
            }

            // Component bitset getters for signatures.
            template <typename TSignature>
            const auto& getRequiresBitset() const noexcept;
            template <typename TSignature>
            const auto& getForbidsBitset() const noexcept;

            // Get a component from data index.
            template <typename TComponent>
            auto& getComponentByDataIndex(DataIndex mX) noexcept;

            // Capacity growth methods.
            void growTo(std::size_t mNewCapacity);
            void growBy(std::size_t mX);
            void growIfNeeded();

            // Get entities from entity indices.
            const auto& getEntity(EntityIndex mX) const noexcept;
            auto& getEntity(EntityIndex mX) noexcept;

            // Get component bitset from entity index.
            const auto& getEntityBitset(EntityIndex mX) const noexcept;

            // Get data index from entity index.
            auto getDataIndex(EntityIndex mX) const noexcept;

            // Handle data getters.
            auto& getHandleData(HandleDataIndex mX) noexcept;
            auto& getHandleData(EntityIndex mX) noexcept;
            auto& getHandleData(const SlimHandle& mX) noexcept;
            const auto& getHandleData(HandleDataIndex mX) const noexcept;
            const auto& getHandleData(EntityIndex mX) const noexcept;
            const auto& getHandleData(const SlimHandle& mX) const noexcept;

            // Call a function passing components matching a signature.
            template <typename TSignature, typename TFunction>
            void expandMatchCall(
                EntityIndex mX, TFunction&& mFunction) noexcept;

            // Refresh the manager and return "one past last alive entity"
            // index.
            auto refreshImpl() noexcept;

        public:
            // Constructors.
            Manager();

            // Checks slim handle validity.
            auto isHandleValid(const SlimHandle& mX) const noexcept;

            // Gets entiy index from a slim handle.
            auto getEntityIndex(const SlimHandle& mX) const noexcept;

            // Count elements.
            auto getEntityCount() const noexcept;
            auto getComponentCount() const noexcept;
            auto getCapacity() const noexcept;

            // Matches signature?
            template <typename TSignature>
            auto matchesSignatureImpl(const EntityBitset& mEntityBitset,
                NoForbiddenElements) const noexcept;
            template <typename TSignature>
            auto matchesSignatureImpl(const EntityBitset& mEntityBitset,
                HasForbiddenElements) const noexcept;
            template <typename TSignature>
            auto matchesSignature(EntityIndex mX) const noexcept;
            template <typename TSignature>
            auto matchesSignature(const SlimHandle& mX) const noexcept;

            // Matches system?
            template <template <typename> class TSystem>
            auto matchesSystem(EntityIndex mX) const noexcept;
            template <template <typename> class TSystem>
            auto matchesSystem(const SlimHandle& mX) const noexcept;

            // Is entity alive?
            auto isAlive(EntityIndex mX) const noexcept;
            auto isAlive(const SlimHandle& mX) const noexcept;

            // Is entity dead?
            auto isDead(EntityIndex mX) const noexcept;
            auto isDead(const SlimHandle& mX) const noexcept;

            // Kill entity.
            auto kill(EntityIndex mX) noexcept;
            auto kill(const SlimHandle& mX) noexcept;

            // Has tag?
            template <typename TTag>
            auto hasTag(EntityIndex mX) const noexcept;
            template <typename TTag>
            auto hasTag(const SlimHandle& mX) const noexcept;

            // Adds a tag.
            template <typename TTag>
            void addTag(EntityIndex mX) noexcept;
            template <typename TTag>
            void addTag(const SlimHandle& mX) noexcept;

            // Removes a tag.
            template <typename TTag>
            void removeTag(EntityIndex mX) noexcept;
            template <typename TTag>
            void removeTag(const SlimHandle& mX) noexcept;

            // Has component?
            template <typename TComponent>
            auto hasComponent(EntityIndex mX) const noexcept;
            template <typename TComponent>
            auto hasComponent(const SlimHandle& mX) const noexcept;

            // Adds a component.
            template <typename TComponent, typename... Ts>
            auto& emplaceComponent(EntityIndex mX, Ts&&... mXs) noexcept;
            template <typename TComponent, typename... Ts>
            auto& emplaceComponent(const SlimHandle& mX, Ts&&... mXs) noexcept;

            // Removes a component.
            template <typename TComponent>
            void removeComponent(EntityIndex mX) noexcept;
            template <typename TComponent>
            void removeComponent(const SlimHandle& mX) noexcept;

            // Gets a reference to a component.
            template <typename TComponent>
            auto& getComponent(EntityIndex mX) noexcept;
            template <typename TComponent>
            auto& getComponent(const SlimHandle& mX) noexcept;

            // Iterates over an entity's components.
            template <typename TFunction>
            void forComponents(TFunction&& mFunction, EntityIndex mX) noexcept;
            template <typename TFunction>
            void forComponents(
                TFunction&& mFunction, const SlimHandle& mX) noexcept;

            // Clear manager.
            // TODO:
            void clear();

            // Entity creation methods.
            auto createIndex();
            auto createSlimHandle();
            auto createFatHandle();

            // Call function on all entities matching a signature.
            template <typename TSignature, typename TFunction>
            void forEntitiesMatchingSignature(TFunction&& mFunction) noexcept;

            // Call function on all entities matching a system.
            template <template <typename> class TSystem, typename TFunction>
            void forEntitiesMatching(TFunction&& mFunction) noexcept;

            // Call system function.
            // TODO: rename
            template <template <typename> class TSystem, typename TFunction>
            void forSystem0(TFunction&& mFunction);

            // Call system member function.
            // TODO: same TArgs in global function call
            template <template <typename> class TSystem, typename... Ts,
                typename... TArgs>
            void forSystem(
                void (TSystem<ThisType>::*mFunction)(Ts...), TArgs&&... mArgs);

            // Get reference to system.
            template <template <typename> class TSystem>
            auto& getSystem() noexcept;

            // Refresh the manager.
            void refresh() noexcept;

            void touchAliveEntity(EntityIndex mX)
            {
                if(!TSettings::useCache) return;

                MPL::forTypes<typename SignatureConfig::AsTypeList>(
                    [this, mX](auto mSignature)
                    {
                        using SigType = ECS_TYPE(mSignature);

                        if(this->matchesSignature<SigType>(mX))
                            entityCache.template add<SigType>(mX);
                    });
            }


            void touchDeadEntity(EntityIndex) noexcept {}

            void refreshHandle(EntityIndex mX) noexcept
            {
                getHandleData(mX).entityIndex = mX;
            }

            void invalidateHandle(EntityIndex mX) noexcept
            {
                ++getHandleData(mX).counter;
            }
        };
    }
}
