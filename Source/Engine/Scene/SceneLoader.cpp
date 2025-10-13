#include "Engine/Precompiled/EnginePch.hpp"
#include "SceneLoader.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Utility/Win/WinConsole.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Engine/Navmesh/NavmeshLoader.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include <fstream>

namespace Simple
{

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

		const std::vector<std::string> missingKeys = ReturnDifferenceBetweenVectors(itemKeys, allPropertyKeys);
		nlohmann::json allProperties = memberJson;

		for (size_t h = 0; h < missingKeys.size(); ++h)
		{
			if (allProperties.contains(missingKeys[h]))
			{
				allProperties.erase(missingKeys[h]);
			}
		}

		memberJson = allProperties;

		return true;
	}

	static void EraseMissingElementFromJSON(const nlohmann::json& jsonData, const std::filesystem::path& absolutePath, const size_t entityIndex, const size_t componentIndex)
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

	static void SaveComponents(ECS::ConstEntityView entityView, nlohmann::ordered_json& entityJson, const DataTypeRegistry& dataTypeRegistry)
	{
		size_t componentCount = 0;

		for (auto [typeInfo, componentPtr] : entityView)
		{
			const DataType* componentDataType = dataTypeRegistry.Find(DataTypeID{ typeInfo.hash_code() });

			nlohmann::ordered_json& componentJson = entityJson["Components"][componentCount];

			componentJson["Name"] = componentDataType->name;
			componentJson["Properties"] = dataTypeRegistry.SaveDataJSON(*componentDataType, componentPtr);

			++componentCount;
		}
	}

	static void SaveECS(const ECS& ecs, nlohmann::ordered_json& sceneJson, const DataTypeRegistry& dataTypeRegistry)
	{
		auto& entitiesJSON = sceneJson["Entities"];

		std::size_t i = 0;
		for (auto entityView : ecs.ViewEntities())
		{
			nlohmann::ordered_json& entityJson = entitiesJSON[i];
			entityJson["ID"] = entityView.GetEntityID().id;

			SaveComponents(entityView, entityJson, dataTypeRegistry);
			i++;
		}
	}

	void SceneLoader::SaveScene(const Scene& scene, const std::filesystem::path& filePath, const DataTypeRegistry& dataTypeRegistry)
	{
		PROFILER_FUNCTION(profiler::colors::TealA400);
		nlohmann::ordered_json sceneJson;
		SaveECS(scene.GetECS(), sceneJson, dataTypeRegistry);
		sceneJson["NavmeshPath"] = scene.GetNavmeshPath();

		std::ofstream writeFile(std::filesystem::absolute(filePath));
		assert(writeFile.is_open() && "Failed to open the file");

		writeFile << sceneJson.dump(-1);
	}

	static bool LoadECS(ECS& ecs, const nlohmann::json& jsonData, std::filesystem::path path, const Blackboard& blackboard)
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

				const DataTypeID dataTypeID = dataTypeRegistry.Find(std::string_view(componentDataJSON["Name"]));
				if (dataTypeID == InvalidDataTypeID)
				{
					EraseMissingElementFromJSON(jsonData, path, i, j); //NOTE(v11.3.2): Maybe not needed but works for now
					continue;
				}

				const std::vector<DataTypeMemberVariable>& componentProperties = dataTypeRegistry.Find(dataTypeID)->memberVariables;
				void* componentPointer = dataTypeRegistry.AddComponent(dataTypeID, ecs, newEntityID);

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

	static bool LoadSceneInternal(Scene& scene, const nlohmann::json& jsonData, const std::filesystem::path& filePath, const Blackboard& blackboard)
	{
		const bool shouldSave = LoadECS(scene.GetECS(), jsonData, filePath, blackboard);

		if (jsonData.contains("NavmeshPath"))
		{
			const std::filesystem::path navmeshPath = jsonData["NavmeshPath"];
			if (!navmeshPath.empty())
			{
				scene.GetNavmesh() = Navmesh(NavmeshLoader::LoadMesh(navmeshPath));
			}
		}
		return shouldSave;
	}

	void SceneLoader::LoadScene(Scene& scene, const std::filesystem::path& filePath, const Blackboard& blackboard)
	{
		PROFILER_FUNCTION(profiler::colors::TealA400);
		const auto jsonData = FileUtility::GetDataAsJson(std::filesystem::absolute(filePath));

		if (jsonData.has_value())
		{
			const bool shouldSave = LoadSceneInternal(scene, jsonData.value(), filePath, blackboard);

			if (shouldSave)
			{
				SaveScene(scene, filePath, blackboard.Get<Key_DataTypeRegistry>());
				Simple::Console::Print(ConvertFilePathToPrettyName(filePath), Simple::ConsoleTextColor::Red, false);
				Simple::Console::Print(" has been updated due to changes in the source code", Simple::ConsoleTextColor::White, true);
			}
		}
		else
		{
			Console::Print(filePath.string() + " does not exist", ConsoleTextColor::Red);
		}
	}

	void SceneLoader::CreateNewSceneFile(const std::filesystem::path& filePath)
	{
		if (filePath.extension() != ".json")
		{
			assert(false && "Wrong Scene file extension");
			return;
		}
		std::ofstream writeFile(filePath);
		assert(writeFile.is_open() && "Failed to open the file");

		nlohmann::json json;
		writeFile << json.dump(-1);
	}
}