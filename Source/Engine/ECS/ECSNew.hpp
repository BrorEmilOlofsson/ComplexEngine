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

#include "Engine/ECS/ECSRegistryNew.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/ECS/EntitySerializationIDGenerator.hpp"
#include "Engine/ECS/ComponentPool.hpp"
#include "Engine/Utility/TypeList.hpp"
#include "Engine/Utility/DynamicBitset.hpp"


namespace CLX
{

    struct ECSArchetype final
    {
        [[nodiscard]] size_t GetPoolIndex(const size_t componentTypeIndex) const
        {
            const auto it = std::ranges::find(componentTypeIndices, componentTypeIndex);
            ASSERT(it != componentTypeIndices.end(), "Component type is not part of this archetype");
            return static_cast<size_t>(std::distance(componentTypeIndices.begin(), it));
        }

        [[nodiscard]] ComponentPoolSBO& GetPoolByTypeIndex(const size_t componentTypeIndex)
        {
            return pools[GetPoolIndex(componentTypeIndex)];
        }

        [[nodiscard]] const ComponentPoolSBO& GetPoolByTypeIndex(const size_t componentTypeIndex) const
        {
            return pools[GetPoolIndex(componentTypeIndex)];
        }

        void ResizePoolIndices(const size_t entityCount)
        {
            for (ComponentPoolSBO& pool : pools)
            {
                pool.ResizeComponentIndices(entityCount);
            }
        }

        std::vector<size_t> componentTypeIndices;
        std::vector<ComponentPoolSBO> pools;
    };

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

        using ValueType = std::tuple<std::add_lvalue_reference_t<Types>...>;
        using PrimaryType = std::tuple_element_t<0, std::tuple<Types...>>;

        MultiComponentIterator() = default;

        MultiComponentIterator(ECSArchetype* archetype, size_t primaryIndex, std::array<size_t, TypeCount> componentIndices)
            : mArchetype(archetype)
            , mPrimaryIndex(primaryIndex)
            , mComponentIndices(componentIndices)
            , mIterator(GetPrimaryComponentPool().Iterate<PrimaryType>().begin())
            , mEntityIDs(GetPrimaryComponentPool().GetEntityIDs())
        {}

        MultiComponentIterator& operator++()
        {
            ++mIterator;
            return *this;
        }

        [[nodiscard]] ValueType operator*() const
        {
            EntityID entityID = GetCurrentEntityID();
            return ValueType
            (
                (*reinterpret_cast<Types*>(GetComponentPtrFromPool(mArchetype->pools[GetComponentIndex<Types>()], entityID)))...
            );
        }

        [[nodiscard]] constexpr bool operator==(const MultiComponentIterator& other) const
        {
            return mIterator == other.mIterator;
        }

        [[nodiscard]] constexpr bool operator==(const MultiComponentSentinel&) const
        {
            return mIterator == typename ComponentPoolSBO::ComponentSentinel{};
        }

        //template<typename T>
        //T* GetPtr()
        //{
        //    constexpr std::size_t typeIndex = TypeListIndex<T, Types...>::value;
        //    auto& componentPools = *mComponentPools;
        //    return 
        //}

        ComponentPoolSBO& GetPrimaryComponentPool()
        {
            return mArchetype->pools[mPrimaryIndex];
        }

        static void* GetComponentPtrFromPool(ComponentPoolSBO& pool, EntityID entityID)
        {
            return pool.GetComponent(entityID);
        }

        EntityID GetCurrentEntityID() const
        {
            return mEntityIDs[mIterator.GetIndex()];
        }

        template<typename T>
        [[nodiscard]] size_t GetComponentIndex() const
        {
            return mComponentIndices[TypeListIndex<T, Types...>::value];
        }

    private:

        ECSArchetype* mArchetype;
        size_t mPrimaryIndex = 0;
        std::array<size_t, TypeCount> mComponentIndices{};
        std::span<const EntityID> mEntityIDs;
        typename ComponentPoolSBO::ComponentIterator<PrimaryType> mIterator;
    };

    template<typename... Types>
    class MultiComponentView final
    {
    public:

        MultiComponentView() = delete;
        MultiComponentView(ECSArchetype* archetype, std::array<size_t, sizeof...(Types)> componentIndices)
            : mArchetype(archetype), mPrimaryIndex(componentIndices[0]), mComponentIndices(componentIndices)
        {}

        [[nodiscard]] MultiComponentIterator<Types...> begin()
        {
            return  MultiComponentIterator<Types...>(mArchetype, mPrimaryIndex, mComponentIndices);
        }

        [[nodiscard]] MultiComponentSentinel end()
        {
            return MultiComponentSentinel{};
        }

    private:

        ECSArchetype* mArchetype;
        size_t mPrimaryIndex;
        std::array<size_t, sizeof...(Types)> mComponentIndices;
    };

    class ECSArchetypeSentinel final {};

    template<typename... Types>
    class ECSArchetypeIterator final
    {
    public:

        using ValueType = std::tuple<std::add_lvalue_reference_t<Types>...>;

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

        EntityData& entityData = mEntityData[entityID.id];
        const std::size_t componentTypeIndex = mRegistry.get().GetComponentTypeIndex(typeid(ComponentType));

        auto ensureArchetypePools = [this](const DynamicBitset& archetypeMask) -> ECSArchetype&
            {
                auto [it, inserted] = mArchetypes.try_emplace(archetypeMask);
                if (inserted)
                {
                    ECSArchetype& archetype = it->second;
                    for (std::size_t i = 0; i < archetypeMask.Count(); ++i)
                    {
                        if (!archetypeMask[i])
                        {
                            continue;
                        }

                        archetype.componentTypeIndices.push_back(i);
                        archetype.pools.push_back(mRegistry.get().GetComponentTypes()[i].createComponentPool());
                    }
                }

                it->second.ResizePoolIndices(mEntityData.size());

                return it->second;
            };

        ComponentMaskNew oldMask = entityData.mask;
        ComponentMaskNew newMask = oldMask;
        newMask[componentTypeIndex] = true;

        ECSArchetype& destinationArchetype = ensureArchetypePools(newMask);

        if (oldMask == newMask)
        {
            return destinationArchetype.GetPoolByTypeIndex(componentTypeIndex).AddComponent<ComponentType>(entityID, std::forward<T>(component));
        }

        if (oldMask.Any())
        {
            // Move components from the old archetype to the new archetype
            auto oldIt = mArchetypes.find(oldMask);
            ASSERT(oldIt != mArchetypes.end(), "Entity archetype pools are missing");

            ECSArchetype& sourceArchetype = oldIt->second;
            for (std::size_t sourcePoolIndex = 0; sourcePoolIndex < sourceArchetype.componentTypeIndices.size(); ++sourcePoolIndex)
            {
                const std::size_t sourceTypeIndex = sourceArchetype.componentTypeIndices[sourcePoolIndex];
                if (sourceTypeIndex == componentTypeIndex)
                {
                    continue;
                }

                ComponentPoolSBO& sourcePool = sourceArchetype.pools[sourcePoolIndex];
                void* sourcePtr = sourcePool.GetComponent(entityID);
                ASSERT(sourcePtr != nullptr, "Entity component is missing in source archetype");

                ComponentPoolSBO& destinationPool = destinationArchetype.GetPoolByTypeIndex(sourceTypeIndex);
                destinationPool.AddComponentMoved(entityID, sourcePtr);
                sourcePool.RemoveComponent(entityID);   
            }
        }

        ComponentType& addedComponent = destinationArchetype.GetPoolByTypeIndex(componentTypeIndex).AddComponent<ComponentType>(entityID, std::forward<T>(component));
        entityData.mask = std::move(newMask);
        return addedComponent;
    }

    template<typename T>
    T* ECSNew::GetComponent(EntityID entityID)
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
        return archetype->GetPoolByTypeIndex(componentTypeIndex).GetComponent<ComponentType>(entityID);
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
                constexpr size_t typeCount = sizeof...(Types);
                std::array<size_t, typeCount> componentIndices{};
                ((componentIndices[TypeListIndex<Types, Types...>::value] = archetype.GetPoolIndex(mRegistry.get().GetComponentTypeIndex(typeid(Types)))), ...);
                multiComponentViews.emplace_back(&archetype, componentIndices);
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

        componentType.createComponentPool = []() -> ComponentPoolSBO
            {
                return ComponentPoolSBO::Create<T>();
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