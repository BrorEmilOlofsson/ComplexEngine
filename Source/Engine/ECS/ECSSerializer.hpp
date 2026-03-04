#pragma once
#include "Engine/Asset/EntityCompositionAsset.hpp"
#include <External/nlohmann/json.hpp>

namespace Simple
{

	class ECS;
	class DataTypeRegistry;

	void SaveECS(const ECS& ecs, nlohmann::ordered_json& sceneJson, const DataTypeRegistry& dataTypeRegistry);
	bool LoadECS(ECS& ecs, const nlohmann::json& jsonData, std::filesystem::path path, const Blackboard& blackboard);

	void SaveEntityCompositionAsset(const EntityCompositionAssetHandle& asset, const DataTypeRegistry& dataTypeRegistry);
	void LoadEntityComposition(const std::filesystem::path& path, EntityComposition& entityComposition, const Blackboard& blackboard);

}