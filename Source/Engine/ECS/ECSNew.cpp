#include "Engine/Precompiled/EnginePch.hpp"
#include "ECSNew.hpp"

namespace CLX
{
    ECSNew::ECSNew(const ECSRegistryNew& ecsRegistry, EntitySerializationIDGenerator& entityIDGenerator)
        : mRegistry(ecsRegistry), mEntityIDGenerator(entityIDGenerator)
    {
    }

    EntityID CreateEntityInternal(const EntitySerializationID serializationID, std::queue<uint32_t>& freeEntityIDs, 
        std::vector<ECSNew::EntityData>& entityDatas, const ECSRegistryNew& registry, ECSNew& ecs)
    {
        EntityID entityID;
        if (!freeEntityIDs.empty())
        {
            const uint32_t id = freeEntityIDs.front();
            entityID = EntityID
            {
                .id = id,
                .generation = entityDatas[id].generation
            };
            ECSNew::EntityData& entityData = entityDatas[entityID.id];
            entityData.mask = ComponentMaskNew();
            entityData.isActive = true;
            entityData.serializationID = serializationID;
            freeEntityIDs.pop();
        }
        else
        {
            entityID = EntityID
            { 
                .id = static_cast<uint32_t>(entityDatas.size()),
                .generation = 0
            };
            ECSNew::EntityData& entityData = entityDatas.emplace_back();
            entityData.serializationID = serializationID;
            //for (auto& componentPool : mComponentPools)
            //{
            //    componentPool.ResizeComponentIndices(size(entityDatas));
            //}
        }

        registry.InitializeEntity(ecs, entityID);

        return entityID;
    }

    EntityID ECSNew::CreateEntity()
    {
        return CreateEntityInternal(
            mEntityIDGenerator.get().Generate(), 
            mFreeEntityIDs, 
            mEntityData, 
            mRegistry.get(), 
            *this
        );
    }

    EntityID ECSNew::CreateEntity(const EntitySerializationID serializationID)
    {
        mEntityIDGenerator.get().MarkIDAsUsed(serializationID);
        return CreateEntityInternal(serializationID, mFreeEntityIDs, mEntityData, mRegistry.get(), *this);
    }
}