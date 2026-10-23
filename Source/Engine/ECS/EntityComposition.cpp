#include "Engine/Precompiled/EnginePch.hpp"
#include "EntityComposition.hpp"
#include "ECSRegistry.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"

namespace CLX
{

	EntityComposition::EntityComposition(ECSOwningHandle ecsOwningHandle, EntityID rootEntityID)
		: mECSHandle(std::move(ecsOwningHandle))
		, mRootEntityID(rootEntityID)
	{
        ASSERT(mRootEntityID != InvalidEntityID, "Invalid root entity ID");
		ASSERT(GetParentEntity(mECSHandle.Get(), mRootEntityID) == InvalidEntityID, "Root entity has a parent");
	}

	EntityID EntityComposition::GetRootEntity() const noexcept
	{
		return mRootEntityID;
	}
}