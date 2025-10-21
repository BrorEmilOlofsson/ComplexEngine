#include "Engine/Precompiled/EnginePch.hpp"
#include "EntityComposition.hpp"

namespace Simple
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