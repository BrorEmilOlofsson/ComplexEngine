#pragma once
#include "Engine/Asset/EntityCompositionAsset.hpp"
#include "Engine/ECS/ECS.hpp"

namespace CLX
{

	void UpdateEntityIDs(const ECS& previousECS, ECS& ecs, const EntityIDConverter& entityIDConverter);
}