#include "Engine/Precompiled/EnginePch.hpp"
#include "SceneManager.hpp"
#include "Engine/ECS/ECS.hpp"
#include "External/profiler.h"
#include "External/nlohmann/json.hpp"
#include "Engine/Scene/SceneLoader.hpp"
#include "Engine/Utility/Algorithm.hpp"
#include <fstream>

namespace Simple
{

	void SceneManager::Clear()
	{
		mScenes.clear();
		mCurrentSceneInfo = nullptr;
	}

	void SceneManager::Init(std::weak_ptr<Blackboard> blackboard)
	{
		mBlackboard = blackboard;
		LoadSettingsFromJson();
#ifndef _EDITOR
		BeginPlay();
#endif
	}

	void SceneManager::Update(const float deltaTime)
	{
		mBlackboard.lock()->Insert<Key_SceneSettings>(mSettings);
		if (mIsPlaying)
		{
			GetCurrentScene().Update(deltaTime);
		}
		else
		{
			GetCurrentScene().EditorUpdate();
		}
	}

	void SceneManager::Render()
	{
		GetCurrentScene().Render();
	}

	void SceneManager::BeginPlay()
	{
		mIsPlaying = true;
		GetCurrentScene().BeginPlay();
	}

	void SceneManager::EndPlay()
	{
		GetCurrentScene().EndPlay();
		mIsPlaying = false;
	}

	void SceneManager::ChangeScene(const std::filesystem::path& scenePath)
	{
		if (!mSceneInfos.contains(scenePath))
		{
			AddScene(scenePath);
		}

		mOnScenePreLoad();
		mCurrentSceneInfo = &mSceneInfos.at(scenePath);
		mOnScenePostLoad(GetCurrentScene());
	}

	void SceneManager::ChangeSceneName(const std::string& newSceneName)
	{
		SceneInfo newSceneInfo;

		newSceneInfo.id = mCurrentSceneInfo->id;
		newSceneInfo.name = newSceneName;
		newSceneInfo.absolutePath = std::filesystem::path(SIMPLE_DIR_SCENES) / (newSceneName + ".scene");

		std::filesystem::rename(mCurrentSceneInfo->absolutePath, newSceneInfo.absolutePath);

		mSceneInfos.erase(mCurrentSceneInfo->absolutePath);
		mSceneInfos.emplace(newSceneInfo.absolutePath, newSceneInfo);

		mCurrentSceneInfo = &mSceneInfos.at(newSceneInfo.absolutePath);
	}

	void SceneManager::CreateNewScene(const std::filesystem::path& filePath)
	{
		SceneLoader::CreateNewSceneFile(filePath);
	}

	void SceneManager::AddOnScenePreLoadFunction(std::function<void()> function)
	{
		mOnScenePreLoad.Add(std::move(function));
	}

	void SceneManager::AddOnScenePostLoadFunction(std::function<void(Scene&)> function)
	{
		mOnScenePostLoad.Add(std::move(function));
	}

	void SceneManager::ReloadSceneFromFile(const std::filesystem::path& scenePath)
	{
		if (mSceneInfos.contains(scenePath))
		{
			mScenes.erase(mSceneInfos.at(scenePath).id);
			LoadAndInitScene(scenePath);
		}
	}

	const SceneInfo* SceneManager::GetCurrentSceneInfo() const
	{
		return mCurrentSceneInfo;
	}

	Scene& SceneManager::GetCurrentScene()
	{
		return mScenes.at(mCurrentSceneInfo->id);
	}

	const Scene& SceneManager::GetCurrentScene() const
	{
		return mScenes.at(mCurrentSceneInfo->id);
	}

	void SceneManager::LoadSettingsFromJson()
	{
		const std::filesystem::path filename = std::filesystem::absolute(SIMPLE_SETTINGS_GAME);

		std::ifstream file(filename);
		assert(file.is_open() && "Failed To Open File");

		const nlohmann::json jsonData = nlohmann::json::parse(file);
		const nlohmann::json gameSettings = jsonData["Game_Settings"];
		file.close();

		const std::filesystem::path sceneFilePath = std::filesystem::absolute(std::filesystem::path(SIMPLE_DIR_SCENES) / gameSettings["Start_Scene_RelativePath"]);
		std::ifstream sceneFile(sceneFilePath);

		if (!sceneFile.is_open())
		{
			std::ofstream writeFile(sceneFilePath);
			assert(writeFile.is_open() && "Failed to create the file");

			const nlohmann::json emptyJson = {};
			writeFile << emptyJson.dump(-1);
		}

		
		LoadDefaultScene(sceneFilePath);
	}

	void SceneManager::LoadDefaultScene(const std::filesystem::path& defaultScenePath)
	{
		ChangeScene(defaultScenePath);
	}

	bool SceneManager::LoadAndInitScene(const std::filesystem::path& scenePath)
	{
		const bool success = mScenes.try_emplace(mSceneInfos.at(scenePath).id, mBlackboard).second;

		if (!success)
		{
			assert(false && "Failed to AddScene");
			return false;
		}

		Scene& scene = mScenes.at(mSceneInfos.at(scenePath).id);

		SceneLoader::LoadScene(scene, scenePath, *mBlackboard.lock());

		return true;
	}

	bool SceneManager::AddScene(const std::filesystem::path& sceneName)
	{
		SceneInfo sceneInfo;

		sceneInfo.id = mNextSceneID++;
		sceneInfo.name = ConvertFilePathToPrettyName(sceneName, false);
		sceneInfo.absolutePath = std::filesystem::absolute(std::filesystem::path(SIMPLE_DIR_SCENES)) / (sceneName);

		mSceneInfos[sceneInfo.absolutePath] = sceneInfo;

		return LoadAndInitScene(sceneInfo.absolutePath);
	}

	bool SceneManager::IsPlaying() const
	{
		return mIsPlaying;
	}

	SceneSettings& SceneManager::GetSettings()
	{
		return mSettings;
	}

	const SceneSettings& SceneManager::GetSettings() const
	{
		return mSettings;
	}
}