#pragma once
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Math/Transform3.hpp"

namespace CLX
{

	struct TransformEntityData final
	{
		EntityID entityID;
		Transform savedTransform;
		bool isDraggingEntity = false;
	};
}