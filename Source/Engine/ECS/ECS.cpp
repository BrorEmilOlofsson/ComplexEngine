#include "Engine/Precompiled/EnginePch.hpp"
#include "ECS.hpp"

namespace CLX
{
    /*ECS::ECS()
    {
        myEntitiesByMask[ComponentMask()];
    }*/

    ECS::ECS(const ECSRegistry& ecsRegistry)
        : mRegistry(ecsRegistry)
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
            mFreeEntityIDs.pop();
        }
        else
        {
            entityID = EntityID{ static_cast<uint32_t>(mEntityData.size()) };
            mEntityData.emplace_back();
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

        for (auto [typeInfo, _] : EntityView(this, entityID))
        {
            const ECSComponentType& componentType = mRegistry.GetComponentType(GetDataTypeID(typeInfo));
            componentType.removeComponentFunction(*this, entityID);
        }

        UpdateEntityMask(entityID, ComponentMask());
        mFreeEntityIDs.push(entityID.id);
    }

    EntityID ECS::CopyEntity(const EntityID entityID, ECS& targetECS) const
    {
        ConstEntityView entityView(this, entityID);

        const EntityID createdEntityID = targetECS.CreateEntity();

        for (auto [typeInfo, componentPtr] : entityView)
        {
            const ECSComponentType& componentType = mRegistry.GetComponentType(GetDataTypeID(typeInfo));

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

        for (auto [typeInfo, componentPtr] : sourceEntityView)
        {
            const ECSComponentType& componentType = mRegistry.GetComponentType(GetDataTypeID(typeInfo));

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

    void ECS::RemoveComponent(const EntityID entityID, const DataTypeID& typeIndex)
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

    const ECSRegistry& ECS::GetRegistry() const
    {
        return mRegistry;
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