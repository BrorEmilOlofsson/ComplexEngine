#pragma once
#include "Utility/Win/WinConsole.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Engine/Scene/SceneLoader.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Utility/Algorithm.hpp"
#include <functional>
#include <fstream>

namespace Simple
{
	class SceneFileFunctions
	{
	public:

		static std::function<void()> Save(const SceneManager* sceneManager, const class DataTypeRegistry* dataTypeRegistry)
		{
			return [sceneManager, dataTypeRegistry]() -> void
				{
					const SceneInfo* sceneInfo = sceneManager->GetCurrentSceneInfo();
					SceneLoader::SaveScene(sceneManager->GetCurrentScene(), sceneInfo->relativePath, *dataTypeRegistry);

					Console::Print("Scene ", ConsoleTextColor::White, false);
					Console::Print(sceneInfo->name.c_str(), ConsoleTextColor::Green, false);
					Console::Print(" has been saved!", ConsoleTextColor::White, true);
				};
		}

		static std::function<void(const std::string&)> Load(SceneManager* sceneManager)
		{
			return [sceneManager](const std::string& aString) -> void
				{
					const std::filesystem::path scenePath = std::filesystem::path(SIMPLE_DIR_SCENES) / aString;
					sceneManager->ChangeScene(scenePath);
					Console::Print("Loaded scene ", ConsoleTextColor::White, false);
					Console::Print(aString.c_str(), ConsoleTextColor::Green, false);
					Console::Print("!", ConsoleTextColor::White, true);
				};
		}

		static std::function<void()> CreateNew(SceneManager* sceneManager)
		{
			return [sceneManager]() -> void
				{
					const std::filesystem::path absolutePath = std::filesystem::absolute(AppendCounterIfAlreadyExist(std::string(SIMPLE_DIR_SCENES) + "\\" + std::string(SIMPLE_FILENAME_NEWSCENE)));
					const std::filesystem::path relativePath = ConvertAbsolutePathToRelativePath(absolutePath);

					sceneManager->CreateNewScene(absolutePath);
					sceneManager->ChangeScene(relativePath);

					Console::Print("New scene ", ConsoleTextColor::White, false);
					Console::Print(sceneManager->GetCurrentSceneInfo()->name.c_str(), ConsoleTextColor::Green, false);
					Console::Print(" has been created!", ConsoleTextColor::White, true);
				};
		}

		static std::function<void()> CreateCopy(SceneManager* sceneManager)
		{
			return [sceneManager]() -> void
				{
					const std::filesystem::path absolutePath = sceneManager->GetCurrentSceneInfo()->absolutePath;
					const std::filesystem::path newCopyName = absolutePath.parent_path() / std::filesystem::path(absolutePath.stem().string() + "_Copy" + absolutePath.extension().string());
					const std::filesystem::path newFileName = AppendCounterIfAlreadyExist(newCopyName);
					const std::filesystem::path relativePath = ConvertAbsolutePathToRelativePath(newFileName);

					std::filesystem::copy_file(absolutePath, newFileName, std::filesystem::copy_options::overwrite_existing);
					sceneManager->ChangeScene(relativePath);

					Console::Print("New scene ", ConsoleTextColor::White, false);
					Console::Print(sceneManager->GetCurrentSceneInfo()->name, ConsoleTextColor::Green, false);
					Console::Print(" has been created!", ConsoleTextColor::White, true);
				};
		}

		static std::function<void()> Reload(SceneManager* sceneManager)
		{
			return [sceneManager]() -> void
				{
					sceneManager->ReloadSceneFromFile(sceneManager->GetCurrentSceneInfo()->relativePath);

					Console::Print("Scene ", ConsoleTextColor::White, false);
					Console::Print(sceneManager->GetCurrentSceneInfo()->name, ConsoleTextColor::Green, false);
					Console::Print(" has been reloaded!", ConsoleTextColor::White, true);
				};
		}

		static std::function<void()> SetAsActive(SceneManager* sceneManager)
		{
			return [sceneManager]() -> void
				{
					const SceneInfo* sceneInfo = sceneManager->GetCurrentSceneInfo();
					std::optional<nlohmann::json> jsonData = FileUtility::GetDataAsJson(std::filesystem::absolute(SIMPLE_SETTINGS_GAME));

					if (!jsonData.has_value())
					{
						return;
					}

					jsonData.value()["Game_Settings"]["Start_Scene_RelativePath"] = sceneInfo->relativePath;

					std::ofstream writeFile(std::filesystem::absolute(SIMPLE_SETTINGS_GAME));
					assert(writeFile.is_open() && "Failed to open the file");

					writeFile << jsonData.value();

					Console::Print("Scene ", ConsoleTextColor::White, false);
					Console::Print(sceneInfo->name.c_str(), ConsoleTextColor::Green, false);
					Console::Print(" has been set as start!", ConsoleTextColor::White, true);
				};
		}
	};
}