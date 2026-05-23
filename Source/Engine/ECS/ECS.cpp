#include "Engine/Precompiled/EnginePch.hpp"
#include "ECS.hpp"

namespace CLX
{

    ECS::ECS(const ECSRegistry& ecsRegistry, EntitySerializationIDGenerator& entityIDGenerator)
        : mRegistry(ecsRegistry)
        , mEntityIDGenerator(entityIDGenerator) 
    {
        mEntitiesByMask[ComponentMask()];
        mIterationList.reserve(1000);

        for (const ECSComponentType& componentType : ecsRegistry.GetComponentTypes())
        {
            mComponentPools.push_back(componentType.createComponentPoolFunction());
        }

        for (const auto& registrationFunction : ecsRegistry.GetSystemRegistrationFunctions())
        {
            mSystems.push_back(registrationFunction());
        }
    }

    EntityID ECS::CreateEntity()
    {
        return CreateEntity(mEntityIDGenerator.get().Generate());
    }

    EntityID ECS::CreateEntity(const EntitySerializationID serializationID)
    {
        EntityID entityID;
        if (!mFreeEntityIDs.empty())
        {
            const uint32_t id = mFreeEntityIDs.front();
            entityID = EntityID
            {
                .id = id,
                .generation = mEntityData[id].generation
            };
            EntityData& entityData = mEntityData[entityID.id];
            entityData.mask = ComponentMask();
            entityData.isActive = true;
            entityData.serializationID = serializationID;
            mFreeEntityIDs.pop();
        }
        else
        {
            entityID = EntityID{ static_cast<uint32_t>(mEntityData.size()) };
            mEntityData.emplace_back();
            EntityData& entityData = mEntityData.back();
            entityData.serializationID = serializationID;
            for (auto& componentPool : mComponentPools)
            {
                componentPool.ResizeComponentIndices(size(mEntityData));
            }
        }

        mRegistry.InitializeEntity(*this, entityID);

        return entityID;
    }

    void ECS::DestroyEntity(const EntityID entityID)
    {
        if (!mEntityData[entityID.id].isActive)
        {
            return;
        }
        mEntityData[entityID.id].isActive = false;
        mEntityData[entityID.id].generation++;

        for (const auto& [typeIndex, _] : ConstEntityView(this, entityID))
        {
            const ECSComponentType& componentType = mRegistry.GetComponentType(typeIndex);
            componentType.removeComponentFunction(*this, entityID);
        }

        UpdateEntityMask(entityID, ComponentMask());
        mFreeEntityIDs.push(entityID.id);
    }

    EntityID ECS::CopyEntity(const EntityID entityID, ECS& targetECS) const
    {
        ConstEntityView entityView(this, entityID);

        const EntityID createdEntityID = targetECS.CreateEntity();

        for (auto [typeIndex, componentPtr] : entityView)
        {
            const ECSComponentType& componentType = mRegistry.GetComponentType(typeIndex);

            componentType.addComponentFunction(targetECS, createdEntityID, componentPtr);
        }

        return createdEntityID;
    }

    EntityID ECS::DuplicateEntity(const EntityID entityID)
    {
        return CopyEntity(entityID, *this);
    }

    void ECS::ReplaceEntity(const EntityID replaceEntityID, const ECS& sourceECS, const EntityID sourceEntityID)
    {
        ConstEntityView sourceEntityView(&sourceECS, sourceEntityID);

        mEntityData[replaceEntityID.id] = {};

        for (auto [typeIndex, componentPtr] : sourceEntityView)
        {
            const ECSComponentType& componentType = mRegistry.GetComponentType(typeIndex);

            componentType.addComponentFunction(*this, replaceEntityID, componentPtr);
        }
    }

    void ECS::ActivateEntity(const EntityID entityID)
    {
        mEntityData[entityID.id].isActive = true;
        mEntitiesByMask[mEntityData[entityID.id].mask].push_back(entityID.id);
        mEntityData[entityID.id].generation = entityID.generation;
    }

    void ECS::DeactivateEntity(const EntityID entityID)
    {
        mEntityData[entityID.id].isActive = false;
        std::erase(mEntitiesByMask[mEntityData[entityID.id].mask], entityID.id);
        mEntityData[entityID.id].generation = entityID.generation;
    }

    void ECS::RemoveComponent(const EntityID entityID, const std::type_index typeIndex)
    {
        const std::size_t index = mRegistry.GetComponentTypeIndex(typeIndex);
        GetComponentPool(index).RemoveComponent(entityID);
        UpdateEntityMask(entityID, false, typeIndex);
    }

    ECS::EntityCollectionView ECS::ViewEntities()
    {
        return EntityCollectionView(this);
    }

    ECS::ConstEntityCollectionView ECS::ViewEntities() const
    {
        return ConstEntityCollectionView(this);
    }

    ECS::EntityView ECS::ViewEntity(const EntityID entityID)
    {
        return EntityView(this, entityID);
    }

    ECS::ConstEntityView ECS::ViewEntity(const EntityID entityID) const
    {
        return ConstEntityView(this, entityID);
    }

    const ECSRegistry& ECS::GetRegistry() const
    {
        return mRegistry;
    }

    EntitySerializationIDGenerator& ECS::GetEntityIDGenerator()
    {
        return mEntityIDGenerator;
    }

    void ECS::BeginPlay(const Blackboard& blackboard)
    {
        for (System& system : mSystems)
        {
            system.BeginPlay(*this, blackboard);
        }
    }

    void ECS::EndPlay(const Blackboard& blackboard)
    {
        for (System& system : mSystems)
        {
            system.EndPlay(*this, blackboard);
        }
    }

    void ECS::EarlyUpdate(const float deltaTime, const Blackboard& blackboard)
    {
        for (System& system : mSystems)
        {
            system.EarlyUpdate(*this, deltaTime, blackboard);
        }
    }

    void ECS::Update(const float deltaTime, const Blackboard& blackboard)
    {
        for (System& system : mSystems)
        {
            system.Update(*this, deltaTime, blackboard);
        }
    }

    void ECS::EditorUpdate(const Blackboard& blackboard)
    {
        for (System& system : mSystems)
        {
            system.EditorUpdate(*this, blackboard);
        }
    }

    void ECS::Render(const Blackboard& blackboard) const
    {
        for (const System& system : mSystems)
        {
            system.Render(*this, blackboard);
        }
    }
}