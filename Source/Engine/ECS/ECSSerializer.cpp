#include "Engine/Precompiled/EnginePch.hpp"
#include "ECSSerializer.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include <fstream>
#include <ranges>
#include <algorithm>

namespace CLX
{

	static void SaveComponents(ECS::ConstEntityView entityView, nlohmann::ordered_json& entityJson, const DataTypeRegistry& dataTypeRegistry)
	{
		std::size_t componentCount = 0;

		for (auto [typeInfo, componentPtr] : entityView)
		{
			const DataType* componentDataType = dataTypeRegistry.Find(GetDataTypeID(typeInfo));

			nlohmann::ordered_json& componentJson = entityJson["Components"][componentCount];

			componentJson["Name"] = componentDataType->name;
			componentJson["Properties"] = dataTypeRegistry.SaveDataJSON(*componentDataType, componentPtr);

			++componentCount;
		}
	}

	void SaveECS(const ECS& ecs, nlohmann::ordered_json& json, const DataTypeRegistry& dataTypeRegistry)
	{
		auto& entitiesJSON = json["Entities"];

		std::size_t i = 0;
		for (auto entityView : ecs.ViewEntities())
		{
			nlohmann::ordered_json& entityJson = entitiesJSON[i];
			entityJson["ID"] = entityView.GetEntityID().id;

			SaveComponents(entityView, entityJson, dataTypeRegistry);
			i++;
		}
	}

	[[nodiscard]] static bool IsJSONDataDifferentFromPropertyData(const std::span<const DataTypeMemberVariable> members, nlohmann::json& memberJson)
	{
		if (members.size() == memberJson.size())
		{
			return false;
		}

		auto propertyItems = memberJson.items();

		std::vector<std::string> itemKeys;
		std::vector<std::string> allPropertyKeys;

		for (auto& keyName : propertyItems)
		{
			itemKeys.emplace_back(keyName.key());
		}

		for (auto& keyName : members)
		{
			allPropertyKeys.emplace_back(keyName.name);
		}

		auto missingKeys = itemKeys | std::views::filter([&allPropertyKeys](const std::string& str) -> bool { return std::ranges::find(allPropertyKeys, str) == allPropertyKeys.end(); });
		nlohmann::json allProperties = memberJson;

		for (auto& missingKey : missingKeys)
		{
			allProperties.erase(missingKey);
		}

		memberJson = allProperties;

		return true;
	}

	static void EraseMissingElementFromJSON(const nlohmann::json& jsonData, const std::filesystem::path& absolutePath, const std::size_t entityIndex, const size_t componentIndex)
	{
		nlohmann::json newData = jsonData;
		auto& entityJson = newData["Entities"][entityIndex];

		nlohmann::json entityComponentJson = entityJson;
		entityComponentJson["Components"].erase(componentIndex);

		entityJson = entityComponentJson;
		newData["Entities"][entityIndex] = entityJson;

		std::ofstream writeFile(absolutePath);
		assert(writeFile.is_open() && "Failed to open the file");

		writeFile << newData;
		writeFile.close();
	}

	bool LoadECS(ECS& ecs, const nlohmann::json& jsonData, std::filesystem::path path, const Blackboard& blackboard)
	{
		PROFILER_FUNCTION(profiler::colors::Brick);
		if (!jsonData.contains("Entities") || jsonData.is_null())
		{
			return false;
		}

		const DataTypeRegistry& dataTypeRegistry = blackboard.Get<Key_DataTypeRegistry>();
		bool shouldUpdateJSON = false;

		const nlohmann::json& entitiesJson = jsonData["Entities"];

		for (size_t i = 0; i < entitiesJson.size(); ++i)
		{
			const nlohmann::json& entityData = entitiesJson[i];
			const EntityID loadedEntityID = { entityData["ID"] };

			const EntityID newEntityID = ecs.CreateEntity();

			if (entityData.contains("Components") == false)
			{
				continue;
			}

			for (size_t j = 0; j < entityData["Components"].size(); ++j)
			{
				const nlohmann::json& componentDataJSON = entityData["Components"][j];

				const std::string componentName = componentDataJSON["Name"];
				const DataTypeID dataTypeID = dataTypeRegistry.Find(componentName);
				if (dataTypeID == InvalidDataTypeID)
				{
					EraseMissingElementFromJSON(jsonData, path, i, j);
					continue;
				}

                const ECSRegistry& registry = ecs.GetRegistry();
				const std::vector<DataTypeMemberVariable>& componentProperties = dataTypeRegistry.Find(dataTypeID)->memberVariables;
				void* componentPointer = registry.GetComponentType(dataTypeID.typeIndex).addComponentFunction(ecs, newEntityID, nullptr);

				if (!componentDataJSON.contains("Properties"))
				{
					continue;
				}

				nlohmann::json membersJSON = componentDataJSON["Properties"];

				if (IsJSONDataDifferentFromPropertyData(componentProperties, membersJSON))
				{
					shouldUpdateJSON = true;
				}

				blackboard.Get<Key_DataTypeRegistry>().LoadDataJSON(dataTypeID, componentPointer, membersJSON, blackboard);
			}
		}

		return shouldUpdateJSON;
	}

	static void SaveEntityComposition(const std::filesystem::path& path, const EntityComposition& entityComposition, const DataTypeRegistry& dataTypeRegistry)
	{
		nlohmann::ordered_json json;


		std::filesystem::path absolutePath = std::filesystem::absolute(path);
		std::filesystem::create_directories(absolutePath.parent_path());
		std::ofstream writeFile(absolutePath, std::ios::out);

		if (!writeFile.is_open())
		{
			return;
		}
		const ECS& ecs = entityComposition.GetECS();
		SaveECS(ecs, json, dataTypeRegistry);

		writeFile << json;
		
	}

	void SaveEntityCompositionAsset(const EntityCompositionAssetHandle& asset, const DataTypeRegistry& dataTypeRegistry)
	{
		if (asset)
		{
			SaveEntityComposition(asset.GetRelativePath(), asset.Get(), dataTypeRegistry);
		}
	}

	void LoadEntityComposition(const std::filesystem::path& path, EntityComposition& entityComposition, const Blackboard& blackboard)
	{
		if (!std::filesystem::exists(path))
		{
			return;
		}

		entityComposition.GetECS() = ECS(entityComposition.GetECS().GetRegistry());

		std::ifstream file(path);

		if (!file.is_open())
		{
			return;
		}

		nlohmann::json json = nlohmann::json::parse(file);
		LoadECS(entityComposition.GetECS(), json, path, blackboard);

	}
}