#include "Editor/Precompiled/EditorPch.hpp"
#include "SetEntityTransformCommand.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"

namespace CLX
{

	void SetEntityTransformCommand::Execute() const
	{
		SetEntityTransform(*ecs, entityID, newTransform);
	}

	void SetEntityTransformCommand::Undo() const
	{
		SetEntityTransform(*ecs, entityID, oldTransform);
	}
}