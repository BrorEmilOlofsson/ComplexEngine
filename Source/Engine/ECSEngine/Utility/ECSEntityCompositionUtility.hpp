#pragma once
#include "Utility/Asset/EntityCompositionAsset.hpp"
#include "Engine/ECS/ECS.hpp"

namespace Simple
{

	void UpdateEntityIDs(const ECS& previousECS, ECS& ecs, const EntityIDConverter& entityIDConverter);
}