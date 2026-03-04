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
			entityID = mFreeEntityIDs.front();
			mEntityData[entityID.id] = {};
			mFreeEntityIDs.pop();
		}
		else
		{
			entityID = EntityID{ static_cast<unsigned int>(mEntityData.size()) };
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
		mEntityData[entityID.id].isActive = false;
		UpdateEntityMask(entityID, ComponentMask());
		mFreeEntityIDs.push(entityID);
		for (auto [typeInfo, componentPtr] : EntityView(this, entityID))
		{
			const ECSComponentType& componentType = mRegistry.GetComponentType(typeInfo);
			componentType.removeComponentFunction(*this, entityID);
		}
	}

	EntityID ECS::CopyEntity(const EntityID entityID, ECS& targetECS) const
	{
		ConstEntityView entityView(this, entityID);

		const EntityID createdEntityID = targetECS.CreateEntity();

		for (auto [typeInfo, componentPtr] : entityView)
		{
			const ECSComponentType& componentType = mRegistry.GetComponentType(typeInfo);

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
			const ECSComponentType& componentType = mRegistry.GetComponentType(typeInfo);

			componentType.addComponentFunction(*this, replaceEntityID, componentPtr);
		}
	}

	void ECS::RemoveComponent(const EntityID entityID, const std::type_index& typeIndex)
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