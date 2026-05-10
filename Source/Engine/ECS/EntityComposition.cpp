#include "Engine/Precompiled/EnginePch.hpp"
#include "EntityComposition.hpp"
#include "ECSRegistry.hpp"

namespace CLX
{

	EntityComposition::EntityComposition(ECSManager& ecsManager, ECSID ecsID)
		: mECSHandle(ecsManager, ecsID)
		, mRootEntityID(mECSHandle.Get().CreateEntity())
	{
	}

	EntityID EntityComposition::GetRootEntity() const noexcept
	{
		return mRootEntityID;
	}
}