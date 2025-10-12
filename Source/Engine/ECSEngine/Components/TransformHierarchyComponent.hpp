#pragma once
#include <vector>
#include "Engine/ECS/EntityID.hpp"

namespace Simple
{

	struct TransformHierarchyComponent final
	{
		std::vector<EntityID> children;
		EntityID parent;
	};

	void RegisterTransformHierarchyComponent();
}