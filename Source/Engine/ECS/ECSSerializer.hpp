#pragma once
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include <External/nlohmann/json.hpp>
#include <optional>

namespace CLX
{

	class ECS;

	void SaveECS(const ECS& ecs, nlohmann::ordered_json& sceneJson, const Blackboard& blackboard);
	bool LoadECS(ECS& ecs, const nlohmann::json& jsonData, const std::filesystem::path& path, const Blackboard& blackboard);

	void SaveEntityCompositionAsset(const EntityCompositionAssetHandle& asset, const Blackboard& blackboard);
	std::optional<EntityComposition> LoadEntityComposition(const std::filesystem::path& path, const ECSRegistry& ecsRegistry, EntitySerializationIDGenerator& idGenerator, ECSManager& ecsManager, const Blackboard& blackboard);

}