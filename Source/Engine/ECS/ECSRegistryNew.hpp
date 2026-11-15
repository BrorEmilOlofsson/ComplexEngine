#pragma once
#include <cstdint>
#include <unordered_map>
#include <span>
#include <vector>
#include <memory>
#include <type_traits>

#include "ECSSystem.hpp"
#include "ComponentPoolNew.hpp"
#include "EntityID.hpp"
#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Utility/EnumUtility.hpp"
#include "Engine/Utility/DynamicBitset.hpp"
#include "Engine/Utility/FundamentalFunctionDefinitions.hpp"

namespace CLX
{
    using ComponentMaskNew = DynamicBitset;

    struct ECSRegisterComponentHelperNew
    {
        template<typename T>
        inline static std::size_t ComponentPoolID{ 0 };
    };

    enum class eECSComponentTraitNew : uint8_t
    {
        None = 0,
        Default = 1 << 0,
        NoManualAdd = 1 << 1
    };

    template<>
    struct IsEnumBitfield<eECSComponentTraitNew> : std::true_type {};


    struct ECSComponentTypeNew final
    {
        ComponentPoolNew(*createComponentPool)() = nullptr;
        CopyAssignFunction copyAssign = nullptr;
        void* (*addComponent)(class ECSNew& ecs, const EntityID entityID, const void* defaultValue) = nullptr;
        void(*removeComponent)(class ECSNew& ecs, const EntityID entityID) = nullptr;
        eECSComponentTraitNew traits = eECSComponentTraitNew::None;
    };

    template<typename T>
    concept IsSystemNew = (StaticUpdateFunctionExists<T> || StaticRenderFunctionExists<T>) && sizeof(T) == 1;

    struct ECSArchetype final
    {
        //std::vector<size_t> componentTypeIndices;
        std::vector<std::optional<ComponentPoolNew>> pools;

        [[nodiscard]] constexpr size_t GetValidPoolCount() const
        {
            return std::ranges::count_if(pools, [](const std::optional<ComponentPoolNew>& pool) { return pool.has_value(); });
        }
    };

    class ECSRegistryNew final
    {
    public:

        template<typename T>
        void RegisterComponentType(eECSComponentTraitNew traits = eECSComponentTraitNew::None);

        template<IsSystemNew T>
        void RegisterSystem()
        {
            mSystemRegistrationFunctions.push_back([]() -> System
                {
                    return System(std::type_identity<T>{});
                });
        }

        [[nodiscard]] std::span<const ECSComponentTypeNew> GetComponentTypes() const
        {
            return mComponentTypes;
        }

        [[nodiscard]] size_t GetComponentTypeIndex(const std::type_index typeIndex) const
        {
            return mComponentTypeToIDMap.at(typeIndex);
        }

        template<typename T>
        [[nodiscard]] size_t GetComponentTypeIndex() const
        {
            return ECSRegisterComponentHelperNew::ComponentPoolID<T>;
        }

        [[nodiscard]] const ECSComponentTypeNew& GetComponentType(const std::type_index typeIndex) const
        {
            const std::size_t componentTypeIndex = GetComponentTypeIndex(typeIndex);
            return mComponentTypes[componentTypeIndex];
        }

        [[nodiscard]] const ECSComponentTypeNew& GetComponentType(size_t index) const
        {
            return mComponentTypes[index];
        }

        template<typename T>
        [[nodiscard]] const ECSComponentTypeNew& GetComponentType() const
        {
            return mComponentTypes[GetComponentTypeIndex<T>()];
        }

        [[nodiscard]] std::span<System(* const)()> GetSystemRegistrationFunctions() const
        {
            return mSystemRegistrationFunctions;
        }

        void SetBlackboard(std::shared_ptr<Blackboard> blackboard)
        {
            mBlackboard = blackboard;
        }

        [[nodiscard]] const Blackboard& GetBlackboard() const
        {
            return *mBlackboard;
        }

        void InitializeEntity(class ECSNew& ecs, EntityID entityID) const
        {
            for (const std::type_index& typeIndex : mDefaultComponentTypeIndices)
            {
                const ECSComponentTypeNew& componentType = GetComponentType(typeIndex);
                componentType.addComponent(ecs, entityID, nullptr);
            }
        }

        template<typename... Types> requires (sizeof...(Types) > 0)
            [[nodiscard]] DynamicBitset CreateComponentMaskFromTypeList() const
        {
            DynamicBitset mask = DynamicBitset::FromSize(GetComponentTypes().size(), false);
            ((mask[GetComponentTypeIndex(typeid(Types))] = true), ...);
            return mask;
        }

        [[nodiscard]] ComponentMaskNew CreateEmptyComponentMask() const
        {
            return ComponentMaskNew::FromSize(GetComponentTypes().size(), false);
        }

        [[nodiscard]] std::vector<std::optional<ComponentPoolNew>> CreateComponentPoolsByComponentMask(const ComponentMaskNew& mask) const
        {
            std::vector<std::optional<ComponentPoolNew>> componentPools;
            componentPools.resize(mComponentTypes.size());
            for (std::size_t i = 0; i < mComponentTypes.size(); ++i)
            {
                if (!mask[i])
                {
                    continue;
                }
                const ECSComponentTypeNew& componentType = mComponentTypes[i];
                ASSERT(componentType.createComponentPool != nullptr, "ECSRegistryNew::CreateComponentPools: createComponentPool is nullptr for component type index");
                componentPools[i] = componentType.createComponentPool();
            }
            return componentPools;
        }

        [[nodiscard]] std::vector<std::optional<ComponentPoolNew>> CreateComponentPoolsByIndices(std::span<const size_t> componentIndices) const
        {
            std::vector<std::optional<ComponentPoolNew>> componentPools;
            componentPools.resize(mComponentTypes.size());
            for (const size_t index : componentIndices)
            {
                const ECSComponentTypeNew& componentType = mComponentTypes[index];
                ASSERT(componentType.createComponentPool != nullptr, "ECSRegistryNew::CreateComponentPools: createComponentPool is nullptr for component type index");
                componentPools[index] = componentType.createComponentPool();
            }
            return componentPools;
        }

        [[nodiscard]] ECSArchetype CreateArchetype(const ComponentMaskNew& mask) const
        {
            return ECSArchetype
            {
                //.componentTypeIndices = { },
                .pools = CreateComponentPoolsByComponentMask(mask)
            };
        }

    private:

        std::unordered_map<std::type_index, std::size_t> mComponentTypeToIDMap;
        std::vector<ECSComponentTypeNew> mComponentTypes;
        std::vector<System(*)()> mSystemRegistrationFunctions;
        std::size_t mCurrentID = 0;
        std::shared_ptr<Blackboard> mBlackboard;
        std::vector<std::type_index> mDefaultComponentTypeIndices;
    };
}