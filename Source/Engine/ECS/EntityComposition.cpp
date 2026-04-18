#include "Engine/Precompiled/EnginePch.hpp"
#include "EntityComposition.hpp"
#include "ECSRegistry.hpp"

namespace CLX
{

	EntityComposition::EntityComposition(const ECSRegistry& ecsRegistry)
		: mECS(ecsRegistry)
		, mRootEntityID(mECS.CreateEntity())
	{
	}

	EntityID EntityComposition::GetRootEntity() const noexcept
	{
		return mRootEntityID;
	}
}