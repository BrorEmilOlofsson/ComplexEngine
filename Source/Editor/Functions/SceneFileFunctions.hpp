#pragma once
#include "Engine/Utility/Win/WinConsole.hpp"
#include "Engine/Utility/File/FileUtility.hpp"
#include "Engine/Scene/SceneLoader.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Utility/Algorithm.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include <functional>
#include <fstream>

namespace CLX
{
    class SceneFileFunctions
    {
    public:

        static std::function<void()> Save(const SceneManager* sceneManager, const class DataTypeRegistry* dataTypeRegistry)
        {
            return [sceneManager, dataTypeRegistry]() -> void
                {
                    auto sceneAsset = sceneManager->GetActiveScene();
                    if (!sceneAsset)
                    {
                        Console::Print("Failed to save the scene! No active scene found.", ConsoleTextColor::Red, true);
                        return;
                    }

                    const Scene& activeScene = *sceneAsset.Get();
                    SceneLoader::SaveScene(activeScene, sceneAsset.GetRelativePath(), *dataTypeRegistry);


                    Console::Print("Scene ", ConsoleTextColor::White, false);
                    Console::Print(activeScene.GetName(), ConsoleTextColor::Green, false);
                    Console::Print(" has been saved!", ConsoleTextColor::White, true);
                };
        }

        static std::function<void(const std::string&)> Load(SceneManager* sceneManager, std::weak_ptr<AssetManager> assetManager)
        {
            return [sceneManager, assetManager](const std::string& name) -> void
                {
                    const std::filesystem::path scenePath = std::filesystem::path(SIMPLE_DIR_SCENES) / name;
                    sceneManager->ChangeScene(assetManager.lock()->GetScene(scenePath));
                    Console::Print("Loaded scene ", ConsoleTextColor::White, false);
                    Console::Print(name, ConsoleTextColor::Green, false);
                    Console::Print("!", ConsoleTextColor::White, true);
                };
        }

        static std::function<void()> CreateNew(SceneManager* sceneManager, std::weak_ptr<AssetManager> assetManager)
        {
            return [sceneManager, assetManager]() -> void
                {
                    const std::filesystem::path absolutePath = std::filesystem::absolute(AppendCounterIfAlreadyExist(std::filesystem::path(SIMPLE_DIR_SCENES) / std::filesystem::path(SIMPLE_FILENAME_NEWSCENE)));

                    SceneAssetHandle sceneAsset = assetManager.lock()->CreateScene(absolutePath);
                    //sceneManager->CreateNewScene(absolutePath);
                    sceneManager->ChangeScene(sceneAsset);

                    Console::Print("New scene ", ConsoleTextColor::White, false);
                    Console::Print(sceneManager->GetActiveScene()->GetName(), ConsoleTextColor::Green, false);
                    Console::Print(" has been created!", ConsoleTextColor::White, true);
                };
        }

        static std::function<void()> CreateCopy(SceneManager* sceneManager, std::weak_ptr<AssetManager> assetManager)
        {
            return [sceneManager, assetManager]() -> void
                {
                    const std::filesystem::path absolutePath = std::filesystem::absolute(sceneManager->GetActiveScene().GetRelativePath());
                    const std::filesystem::path newCopyName = absolutePath.parent_path() / std::filesystem::path(absolutePath.stem().string() + "_Copy" + absolutePath.extension().string());
                    const std::filesystem::path newFilePath = AppendCounterIfAlreadyExist(newCopyName);

                    std::filesystem::copy_file(absolutePath, newFilePath, std::filesystem::copy_options::overwrite_existing);
                    assetManager.lock()->LoadAsset(newFilePath);
                    SceneAssetHandle sceneAsset = assetManager.lock()->GetScene(newFilePath);
                    sceneManager->ChangeScene(sceneAsset);

                    Console::Print("New scene ", ConsoleTextColor::White, false);
                    Console::Print(sceneManager->GetActiveScene()->GetName(), ConsoleTextColor::Green, false);
                    Console::Print(" has been created!", ConsoleTextColor::White, true);
                };
        }

        static std::function<void()> Reload(SceneManager* sceneManager, std::weak_ptr<AssetManager> assetManager)
        {
            return [sceneManager, assetManager]() -> void
                {
                    const SceneAssetHandle activeSceneAsset = sceneManager->GetActiveScene();
                    assetManager.lock()->LoadAsset(activeSceneAsset.GetRelativePath());

                    Console::Print("Scene ", ConsoleTextColor::White, false);
                    Console::Print(sceneManager->GetActiveScene()->GetName(), ConsoleTextColor::Green, false);
                    Console::Print(" has been reloaded!", ConsoleTextColor::White, true);
                };
        }

        static std::function<void()> SetAsActive(SceneManager* sceneManager)
        {
            return [sceneManager]() -> void
                {
                    const SceneAssetHandle activeSceneAsset = sceneManager->GetActiveScene();
                    std::filesystem::path relativePath = activeSceneAsset.GetRelativePath();
                    std::optional<nlohmann::json> jsonData = FileUtility::GetDataAsJson(std::filesystem::absolute(SIMPLE_SETTINGS_GAME));

                    if (!jsonData.has_value())
                    {
                        return;
                    }

                    jsonData.value()["Game_Settings"]["Start_Scene_RelativePath"] = relativePath;

                    std::ofstream writeFile(std::filesystem::absolute(SIMPLE_SETTINGS_GAME));
                    assert(writeFile.is_open() && "Failed to open the file");

                    writeFile << jsonData.value();

                    Console::Print("Scene ", ConsoleTextColor::White, false);
                    Console::Print(relativePath.stem().string().c_str(), ConsoleTextColor::Green, false);
                    Console::Print(" has been set as start!", ConsoleTextColor::White, true);
                };
        }
    };
}