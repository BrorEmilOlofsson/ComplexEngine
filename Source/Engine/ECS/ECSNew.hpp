#pragma once
#include <type_traits>
#include <vector>
#include <concepts>
#include <tuple>
#include <utility>
#include <bitset>
#include <array>
#include <ranges>
#include <map>
#include <queue>
#include <optional>

#include "Engine/ECS/ECSRegistryNew.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/ECS/EntitySerializationIDGenerator.hpp"
#include "Engine/ECS/ComponentPoolNew.hpp"
#include "Engine/Utility/TypeList.hpp"
#include "Engine/Utility/DynamicBitset.hpp"


namespace CLX
{

    struct MultiComponentSentinel final
    {

    };

    template<typename... Types>
    class MultiComponentIterator final
    {
    public:

        static constexpr std::size_t TypeCount = sizeof...(Types);
        static_assert(TypeCount > 0, "MultiComponentIterator requires at least one type");
        static_assert((std::same_as<Types, std::remove_reference_t<Types>> && ...), "MultiComponentIterator requires non-reference types");

        using ValueType = std::conditional_t<TypeCount == 1, std::add_lvalue_reference_t<std::tuple_element_t<0, std::tuple<Types...>>>, std::tuple<std::add_lvalue_reference_t<Types>...>>;
        using PrimaryType = std::tuple_element_t<0, std::tuple<Types...>>;

        MultiComponentIterator() = default;

        constexpr MultiComponentIterator(ECSArchetype* archetype, std::array<size_t, TypeCount> componentIndices)
            : mArchetype(archetype)
            , mComponentIndices(componentIndices)
            , mEntityIDs(GetFirstPool().GetComponentIndexToEntityIDList())
            , mIterator(archetype->pools[GetFirstComponentIndex()]->Iterate<PrimaryType>().begin())
        {}

        MultiComponentIterator& operator++()
        {
            ++mIterator;
            return *this;
        }

        [[nodiscard]] ValueType operator*() const
        {
            const EntityID entityID = GetCurrentEntityID();
            return ValueType
            (
                (*reinterpret_cast<Types*>(GetComponentPtrFromPool(*mArchetype->pools[GetComponentIndex<Types>()], entityID)))...
            );
        }

        [[nodiscard]] constexpr bool operator==(const MultiComponentIterator& other) const
        {
            return mIterator == other.mIterator;
        }

        [[nodiscard]] constexpr bool operator==(const MultiComponentSentinel&) const
        {
            return mIterator == typename ComponentPoolNew::ComponentSentinel{};
        }

        static void* GetComponentPtrFromPool(ComponentPoolNew& pool, EntityID entityID)
        {
            return pool.GetComponent(entityID);
        }

        [[nodiscard]] constexpr EntityID GetCurrentEntityID() const
        {
            return mEntityIDs[mIterator.GetIndex()];
        }

        [[nodiscard]] constexpr size_t GetFirstComponentIndex() const
        {
            return mComponentIndices[0];
        }

        [[nodiscard]] constexpr ComponentPoolNew& GetFirstPool()
        {
            return *mArchetype->pools[GetFirstComponentIndex()];
        }

        template<typename T>
        [[nodiscard]] constexpr size_t GetComponentIndex() const
        {
            return mComponentIndices[TypeListIndex<T, Types...>::value];
        }

    private:

        ECSArchetype* mArchetype = nullptr;
        std::array<size_t, TypeCount> mComponentIndices{};
        std::span<const EntityID> mEntityIDs;
        typename ComponentPoolNew::ComponentIterator<PrimaryType> mIterator;
    };

    template<typename... Types>
    class MultiComponentView final
    {
    public:

        constexpr static std::size_t TypeCount = sizeof...(Types);

        MultiComponentView() = delete;
        constexpr MultiComponentView(ECSArchetype* archetype)
            : mArchetype(archetype)
            , mComponentIndices(GetComponentIndices())
        {}

        static constexpr std::array<size_t, TypeCount> GetComponentIndices()
        {
            return { ECSRegisterComponentHelperNew::ComponentPoolID<Types>... };
        }

        [[nodiscard]] MultiComponentIterator<Types...> begin()
        {
            return MultiComponentIterator<Types...>(mArchetype, mComponentIndices);
        }

        [[nodiscard]] MultiComponentSentinel end()
        {
            return MultiComponentSentinel{};
        }

    private:

        ECSArchetype* mArchetype;
        size_t mPrimaryIndex;
        std::array<size_t, TypeCount> mComponentIndices;
    };

    class ECSArchetypeSentinel final {};

    template<typename... Types>
    class ECSArchetypeIterator final
    {
    public:

        static constexpr std::size_t TypeCount = sizeof...(Types);
        using ValueType = std::conditional_t<TypeCount == 1, std::add_lvalue_reference_t<std::tuple_element_t<0, std::tuple<Types...>>>, std::tuple<std::add_lvalue_reference_t<Types>...>>;

        ECSArchetypeIterator() = delete;
        ECSArchetypeIterator(std::vector<MultiComponentView<Types...>>& multiComponentIteratorViews)
            : mMultiComponentIteratorViews(multiComponentIteratorViews)
            , mCurrentIndex(0)
            , mCurrentIterator(mMultiComponentIteratorViews.empty() ? MultiComponentIterator<Types...>() : mMultiComponentIteratorViews[mCurrentIndex].begin())
        {}


        [[nodiscard]] constexpr ValueType operator*()
        {
            return *mCurrentIterator;
        }

        ECSArchetypeIterator& operator++()
        {
            auto& currentView = mMultiComponentIteratorViews[mCurrentIndex];
            ++mCurrentIterator;
            if (mCurrentIterator == currentView.end())
            {
                mCurrentIndex++;
                if (mCurrentIndex < mMultiComponentIteratorViews.size())
                {
                    mCurrentIterator = mMultiComponentIteratorViews[mCurrentIndex].begin();
                }
            }
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(const ECSArchetypeIterator& other) const
        {
            return mCurrentIndex == other.mCurrentIndex;
        }

        [[nodiscard]] constexpr bool operator==(const ECSArchetypeSentinel&) const
        {
            return mCurrentIndex == mMultiComponentIteratorViews.size();
        }

    private:

        std::span<MultiComponentView<Types...>> mMultiComponentIteratorViews;
        size_t mCurrentIndex = 0;
        MultiComponentIterator<Types...> mCurrentIterator;
    };

    template<typename... Types>
    class ECSArchetypeView final
    {
    public:

        ECSArchetypeView() = delete;
        ECSArchetypeView(std::vector<MultiComponentView<Types...>>&& multiComponentIteratorViews)
            : mMultiComponentIteratorViews(std::move(multiComponentIteratorViews))
        {}


        ECSArchetypeIterator<Types...> begin()
        {
            return ECSArchetypeIterator<Types...>(mMultiComponentIteratorViews);
        }

        ECSArchetypeSentinel end()
        {
            return ECSArchetypeSentinel{};
        }

    private:

        std::vector<MultiComponentView<Types...>> mMultiComponentIteratorViews;
    };

    class ECSNew final
    {
    public:

        ECSNew() = delete;
        explicit ECSNew(const ECSRegistryNew& ecsRegistry, EntitySerializationIDGenerator& entityIDGenerator);

        EntityID CreateEntity();
        EntityID CreateEntity(EntitySerializationID serializationID);

        // Adds or replaces a component of type T to the specified entity. If the entity already has a component of type T, it will be replaced with the new component.
        template<typename T>
        T& AddComponent(EntityID entityID, T&& component);

        template<typename T>
        T& AddComponent(EntityID entityID);

        template<typename T>
        void RemoveComponent(EntityID entityID);

        template<typename T>
        T* GetComponent(EntityID entityID);

        template<typename... Types> requires (sizeof...(Types) > 0)
            [[nodiscard]] ECSArchetypeView<Types...> ForEach();

        struct EntityData
        {
            ComponentMaskNew mask = {};
            EntitySerializationID serializationID = InvalidID<EntitySerializationID>();
            uint32_t generation = 0;
            bool isActive = true;
        };

        [[nodiscard]] const EntityData& GetEntityData(const EntityID entityID) const
        {
            return mEntityData[entityID.id];
        }

        [[nodiscard]] ECSArchetype* GetArchetype(const DynamicBitset& mask)
        {
            const auto it = mArchetypes.find(mask);
            if (it == mArchetypes.end())
            {
                return nullptr;
            }
            return &it->second;
        }

        [[nodiscard]] const ECSArchetype* GetArchetype(const DynamicBitset& mask) const
        {
            const auto it = mArchetypes.find(mask);
            if (it == mArchetypes.end())
            {
                return nullptr;
            }
            return &it->second;
        }

        [[nodiscard]] const std::map<DynamicBitset, ECSArchetype>& GetArchetypes() const
        {
            return mArchetypes;
        }

    private:

        std::reference_wrapper<const ECSRegistryNew> mRegistry;
        std::reference_wrapper<EntitySerializationIDGenerator> mEntityIDGenerator;
        std::map<DynamicBitset, ECSArchetype> mArchetypes;
        std::queue<uint32_t> mFreeEntityIDs;
        std::vector<EntityData> mEntityData;
    };

    template<typename T>
    T& ECSNew::AddComponent(EntityID entityID, T&& component)
    {
        using ComponentType = std::remove_cvref_t<T>;

        ASSERT(static_cast<size_t>(entityID.id) < mEntityData.size(), "ECSNew::AddComponent: Invalid entity ID");

        EntityData& entityData = mEntityData[entityID.id];
        const std::size_t componentTypeIndex = mRegistry.get().GetComponentTypeIndex<ComponentType>();

        auto ensureArchetypePools = [this](const DynamicBitset& archetypeMask) -> ECSArchetype&
            {
                auto [it, inserted] = mArchetypes.try_emplace(archetypeMask);
                if (inserted)
                {
                    ECSArchetype& newArchetype = it->second;
                    newArchetype = mRegistry.get().CreateArchetype(archetypeMask);
                }

                return it->second;
            };

        const ComponentMaskNew oldMask = entityData.mask;
        ComponentMaskNew newMask = oldMask;
        newMask[componentTypeIndex] = true;

        ECSArchetype& destinationArchetype = ensureArchetypePools(newMask);

        if (oldMask == newMask)
        {
            return destinationArchetype.pools[componentTypeIndex]->AddComponent<ComponentType>(entityID, std::forward<T>(component));
        }

        if (oldMask.Any())
        {
            // Move components from the old archetype to the new archetype
            auto oldIt = mArchetypes.find(oldMask);
            ASSERT(oldIt != mArchetypes.end(), "Entity archetype pools are missing");


            ECSArchetype& sourceArchetype = oldIt->second;
            //ASSERT(sourceArchetype.componentTypeIndices.size() == oldMask.Count(), "Source archetype component type indices size does not match old mask count");
            for (size_t componentIndex = 0; componentIndex < sourceArchetype.pools.size(); componentIndex++)
            {
                std::optional<ComponentPoolNew>& sourcePoolOpt = sourceArchetype.pools[componentIndex];
                if (!sourcePoolOpt.has_value())
                {
                    continue;
                }
                ComponentPoolNew& sourcePool = *sourcePoolOpt;
                void* sourcePtr = sourcePool.GetComponent(entityID);
                ASSERT(sourcePtr != nullptr, "Entity component is missing in source archetype");

                ComponentPoolNew& destinationPool = *destinationArchetype.pools[componentIndex];
                destinationPool.AddComponentMoved(entityID, sourcePtr);
                sourcePool.RemoveComponent(entityID);
            }
        }

        ComponentType& addedComponent = destinationArchetype.pools[componentTypeIndex]->AddComponent<ComponentType>(entityID, std::forward<T>(component));
        entityData.mask = std::move(newMask);
        return addedComponent;
    }

    template<typename T>
    T& ECSNew::AddComponent(const EntityID entityID)
    {
        return AddComponent(entityID, T{});
    }

    template<typename T>
    void ECSNew::RemoveComponent(const EntityID entityID)
    {

        auto ensureArchetypePools = [this](const DynamicBitset& archetypeMask) -> ECSArchetype&
            {
                auto [it, inserted] = mArchetypes.try_emplace(archetypeMask);
                if (inserted)
                {
                    ECSArchetype& newArchetype = it->second;
                    newArchetype = mRegistry.get().CreateArchetype(archetypeMask);
                }

                return it->second;
            };

        ASSERT(static_cast<size_t>(entityID.id) < mEntityData.size(), "ECSNew::RemoveComponent: Invalid entity ID");

        const size_t componentTypeIndex = mRegistry.get().GetComponentTypeIndex<T>();
        EntityData& entityData = mEntityData[entityID.id];
        if (!entityData.mask[componentTypeIndex])
        {
            return; // Component not present, nothing to remove
        }
        ComponentMaskNew oldMask = entityData.mask;
        ECSArchetype* sourceArchetypePtr = GetArchetype(oldMask);
        ASSERT(sourceArchetypePtr != nullptr, "Entity archetype pools are missing");
        ECSArchetype& sourceArchetype = *sourceArchetypePtr;
        ComponentMaskNew newMask = oldMask;
        newMask[componentTypeIndex] = false;

        sourceArchetype.pools[componentTypeIndex]->RemoveComponent(entityID);

        if (newMask.Any())
        {
            auto& newArchetype = ensureArchetypePools(newMask);
            for (size_t componentIndex = 0; componentIndex < sourceArchetype.pools.size(); componentIndex++)
            {
                std::optional<ComponentPoolNew>& sourcePoolOpt = sourceArchetype.pools[componentIndex];
                if (!sourcePoolOpt.has_value() || componentIndex == componentTypeIndex)
                {
                    continue;
                }
                ComponentPoolNew& sourcePool = *sourcePoolOpt;
                void* sourcePtr = sourcePool.GetComponent(entityID);
                ASSERT(sourcePtr != nullptr, "Entity component is missing in source archetype");

                ComponentPoolNew& destinationPool = *newArchetype.pools[componentIndex];
                destinationPool.AddComponentMoved(entityID, sourcePtr);
                sourcePool.RemoveComponent(entityID);
            }
        }
        entityData.mask = std::move(newMask);
    }

    template<typename T>
    T* ECSNew::GetComponent(const EntityID entityID)
    {
        using ComponentType = std::remove_cvref_t<T>;
        const std::size_t componentTypeIndex = mRegistry.get().GetComponentTypeIndex(typeid(ComponentType));
        const EntityData& entityData = mEntityData[entityID.id];
        if (!entityData.mask[componentTypeIndex])
        {
            return nullptr;
        }
        ECSArchetype* archetype = GetArchetype(entityData.mask);
        ASSERT(archetype != nullptr, "Entity archetype pools are missing");
        std::optional<ComponentPoolNew>& pool = archetype->pools[componentTypeIndex];
        ASSERT(pool.has_value(), "Component pool is missing for the specified component type");
        return pool->GetComponent<ComponentType>(entityID);
    }

    template<typename... Types> requires (sizeof...(Types) > 0)
        ECSArchetypeView<Types...> ECSNew::ForEach()
    {
        const DynamicBitset mask = mRegistry.get().CreateComponentMaskFromTypeList<Types...>();
        std::vector<MultiComponentView<Types...>> multiComponentViews;
        for (auto& [existingMask, archetype] : mArchetypes)
        {
            if ((existingMask & mask) == mask)
            {
                bool hasAnyComponent = false;
                for (size_t componentIndex = 0; componentIndex < archetype.pools.size(); componentIndex++)
                {
                    std::optional<ComponentPoolNew>& poolOpt = archetype.pools[componentIndex];
                    if (!poolOpt.has_value())
                    {
                        continue;
                    }
                    ComponentPoolNew& pool = *poolOpt;
                    if (pool.GetComponentCount() > 0)
                    {
                        hasAnyComponent = true;
                        break;
                    }
                }
                if (hasAnyComponent)
                {
                    multiComponentViews.emplace_back(&archetype);
                }
            }
        }
        return ECSArchetypeView<Types...>(std::move(multiComponentViews));
    }

    template<typename T>
    void ECSRegistryNew::RegisterComponentType(const eECSComponentTraitNew traits)
    {
        [[maybe_unused]] const bool hasBeenRegistered = mComponentTypeToIDMap.contains(std::type_index(typeid(T)));
        ASSERT(!hasBeenRegistered, "Component type has already been registered");

        ECSComponentTypeNew componentType;

        componentType.traits = traits;

        componentType.createComponentPool = []() -> ComponentPoolNew
            {
                return ComponentPoolNew::Create<T>();
            };

        componentType.copyAssign = CreateCopyAssignFunction<T>();

        //componentType.addComponent = [](ECS& ecs, const EntityID entityID, const void* defaultValuePtr) -> void*
        //    {
        //        if (defaultValuePtr != nullptr)
        //        {
        //            const T& defaultValue = *reinterpret_cast<const T*>(defaultValuePtr);
        //            T& component = ecs.AddComponent<T>(entityID, defaultValue);
        //            return &component;
        //        }
        //        else
        //        {
        //            T& component = ecs.AddComponent<T>(entityID);
        //            return &component;
        //        }
        //    };

        //componentType.removeComponent = [](ECSNew& ecs, const EntityID entityID)
        //    {
        //        ecs.RemoveComponent<T>(entityID);
        //    };

        mComponentTypes.push_back(componentType);

        mComponentTypeToIDMap.emplace(std::type_index(typeid(T)), mCurrentID);

        ECSRegisterComponentHelperNew::ComponentPoolID<T> = mCurrentID;
        mCurrentID++;

        if (HasFlag(traits, eECSComponentTraitNew::Default))
        {
            mDefaultComponentTypeIndices.push_back(std::type_index(typeid(T)));
        }
    }

}