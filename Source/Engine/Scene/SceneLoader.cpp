#include "Engine/Precompiled/EnginePch.hpp"
#include "SceneLoader.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Utility/Win/WinConsole.hpp"
#include "Engine/Utility/File/FileUtility.hpp"
#include "Engine/Navmesh/NavmeshLoader.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Engine/ECS/ECSSerializer.hpp"
#include <fstream>

namespace CLX
{

	void SceneLoader::SaveScene(const Scene& scene, const std::filesystem::path& filePath, const DataTypeRegistry& dataTypeRegistry)
	{
		PROFILER_FUNCTION(profiler::colors::TealA400);
		nlohmann::ordered_json sceneJson;
		SaveECS(scene.GetECS(), sceneJson, dataTypeRegistry);
		sceneJson["NavmeshPath"] = std::filesystem::relative(scene.GetNavmeshPath(), std::filesystem::path(SIMPLE_DIR_ASSETS));

		std::ofstream writeFile(std::filesystem::absolute(filePath));
		assert(writeFile.is_open() && "Failed to open the file");

		writeFile << sceneJson.dump(-1);
	}

	static bool LoadSceneInternal(Scene& scene, const nlohmann::json& jsonData, const std::filesystem::path& filePath, const Blackboard& blackboard)
	{
		const bool shouldSave = LoadECS(scene.GetECS(), jsonData, filePath, blackboard);

		if (jsonData.contains("NavmeshPath"))
		{
			const std::filesystem::path navmeshRelativePath = jsonData.at("NavmeshPath");
			if (!navmeshRelativePath.empty())
			{
				const std::filesystem::path navmeshPath = std::filesystem::path(SIMPLE_DIR_ASSETS) / navmeshRelativePath;
				scene.GetNavmesh() = Navmesh(NavmeshLoader::LoadMesh(navmeshPath));

				scene.SetNavmeshPath(navmeshPath);
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
				Console::Print(ConvertFilePathToPrettyName(filePath), ConsoleTextColor::Red, false);
				Console::Print(" has been updated due to changes in the source code", ConsoleTextColor::White, true);
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