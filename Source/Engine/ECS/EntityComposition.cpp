#include "Engine/Precompiled/EnginePch.hpp"
#include "EntityComposition.hpp"

namespace Simple
{

	EntityComposition::EntityComposition(const ECSRegistry& ecsRegistry)
		: mECS(ecsRegistry)
	{
	}
}