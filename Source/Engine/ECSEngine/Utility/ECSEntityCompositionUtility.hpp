#pragma once
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/ECS/ECSHandle.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/ECSEngine/Components/EntityCompositionInstantiationComponent.hpp"
#include "Editor/EntityCompositionInstantiationManager.hpp"

namespace CLX
{
    [[nodiscard]] inline bool HasVariableEntity(const DataTypeID& dataTypeID)
    {
        return dataTypeID == GetDataTypeID<EntityID>() || dataTypeID == GetDataTypeID<std::vector<EntityID>>();
    }

    inline void RemapComponentEntityIDs(ECS& ecs, const EntityID newEntityID, const auto& oldToNewEntityIDMap, const DataTypeRegistry& dataTypeRegistry)
    {
        for (auto [typeinfo, componentPtr] : ECS::EntityView(&ecs, newEntityID))
        {
            const DataType* dataType = dataTypeRegistry.Find(GetDataTypeID(typeinfo));
            ASSERT(dataType != nullptr);
            auto& members = dataType->memberVariables;

            auto membersFiltered = members | std::views::filter([](const DataTypeMemberVariable& member)
                {
                    return HasVariableEntity(member.dataTypeID);
                });

            auto memberPair = membersFiltered | std::views::transform([&dataTypeRegistry, &componentPtr](const DataTypeMemberVariable& member)
                {
                    return std::pair<std::type_index, void*>{ dataTypeRegistry.Find(member.dataTypeID)->type, componentPtr + std::get<ByteOffset>(member.memberType) };
                })
                | std::ranges::to<std::vector>();

            for (const auto& [memberTypeInfo, memberPtr] : memberPair)
            {
                if (memberTypeInfo == typeid(EntityID))
                {
                    EntityID& entityRef = *static_cast<EntityID*>(memberPtr);
                    if (oldToNewEntityIDMap.contains(entityRef))
                    {
                        entityRef = oldToNewEntityIDMap.at(entityRef);
                    }
                }
                else if (memberTypeInfo == typeid(std::vector<EntityID>))
                {
                    std::vector<EntityID>& entityVectorRef = *static_cast<std::vector<EntityID>*>(memberPtr);
                    for (EntityID& entityIDInVector : entityVectorRef)
                    {
                        if (oldToNewEntityIDMap.contains(entityIDInVector))
                        {
                            entityIDInVector = oldToNewEntityIDMap.at(entityIDInVector);
                        }
                    }
                }
                else
                {
                    ASSERT_NEW(false, "Unexpected type info for member variable when duplicating entity and children");
                }
            }
        }
    }

	void UpdateEntityIDs(const ECS& previousECS, ECS& ecs, const EntityIDConverter& entityIDConverter, const DataTypeRegistry& dataTypeRegistry);

    // Returns root entity of instantiated entity composition
    //EntityID InstantiateEntityComposition(ECSHandle targetECSHandle, const EntityCompositionAssetHandle& compositionAsset);

    inline EntityIDConverter MergeECS(ECS& targetECS, const ECS& sourceECS)
    {
        auto entityCollectionView = sourceECS.ViewEntities();
        std::unordered_map<EntityID, EntityID> entityConverter1(entityCollectionView.GetCount());
        std::unordered_map<EntityID, EntityID> entityConverter2(entityCollectionView.GetCount());
        for (auto entity : entityCollectionView)
        {
            const EntityID createdEntityID = sourceECS.CopyEntity(entity.GetEntityID(), targetECS);
            entityConverter1[entity.GetEntityID()] = createdEntityID;
            entityConverter2[createdEntityID] = entity.GetEntityID();
        }
        return EntityIDConverter(std::move(entityConverter1), std::move(entityConverter2));
    }


    inline EntityID InstantiateEntityComposition(const EntityCompositionAssetHandle compositionAsset, ECS& targetECS, 
        const EntityID parentID, const DataTypeRegistry& dataTypeRegistry)
    {
        const EntityComposition& entityComposition = compositionAsset.Get();
        const EntityIDConverter entityConverter = MergeECS(targetECS, entityComposition.GetECS());
        UpdateEntityIDs(entityComposition.GetECS(), targetECS, entityConverter, dataTypeRegistry);
        const EntityID instantiatedRootEntity = entityConverter.ConvertToTarget(entityComposition.GetRootEntity());
        targetECS.AddComponent(instantiatedRootEntity,
            EntityCompositionInstantiationComponent
            {
                .asset = compositionAsset,
                .mappedEntityID = entityComposition.GetRootEntity()
            });
        for (const EntityID entityID : GetEntityDescendants(targetECS, instantiatedRootEntity))
        {
            auto& compositionComponent = targetECS.AddComponent<EntityCompositionInstantiationComponent>(entityID);
            compositionComponent.asset = compositionAsset;
            compositionComponent.mappedEntityID = entityConverter.ConvertToSource(entityID);
        }

        if (parentID != InvalidEntityID)
        {
            SetParentEntity(targetECS, instantiatedRootEntity, parentID);
        }

        return instantiatedRootEntity;
    }
}