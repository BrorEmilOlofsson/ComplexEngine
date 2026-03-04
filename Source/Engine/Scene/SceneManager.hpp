#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
#include "Engine/Scene/Scene.hpp"
#include "Engine/Utility/EventDispatcher.hpp"
#include "Engine/Scene/SceneSettings.hpp"

namespace CLX
{

	using SceneID = std::size_t;

	struct SceneInfo
	{
		SceneID id = static_cast<size_t>(-1);
		std::string name;
		std::filesystem::path absolutePath;
	};

	class SceneManager final
	{
	public:

		SceneManager() = default;

		void Init(std::weak_ptr<Blackboard> blackboard);
		void Update(const float deltaTime);
		void Render();

		void BeginPlay();
		void EndPlay();

		void ReloadSceneFromFile(const std::filesystem::path& sceneName);

		void ChangeScene(const std::filesystem::path& sceneName);
		void ChangeSceneName(const std::string& newSceneName);
		void CreateNewScene(const std::filesystem::path& filePath);

		void AddOnScenePreLoadFunction(std::function<void()> function);
		void AddOnScenePostLoadFunction(std::function<void(Scene&)> function);

	public:

		[[nodiscard]] const SceneInfo* GetCurrentSceneInfo() const;
		[[nodiscard]] Scene& GetCurrentScene();
		[[nodiscard]] const Scene& GetCurrentScene() const;
		[[nodiscard]] bool IsPlaying() const;

		[[nodiscard]] SceneSettings& GetSettings();
		[[nodiscard]] const SceneSettings& GetSettings() const;

	private:

		void LoadSettingsFromJson();
		void LoadDefaultScene(const std::filesystem::path& defaultScenePath);
		bool LoadAndInitScene(const std::filesystem::path& sceneName);

	private:

		bool AddScene(const std::filesystem::path& sceneName);
		void Clear();

	private:

		std::unordered_map<std::filesystem::path, SceneInfo> mSceneInfos;
		std::unordered_map<SceneID, Scene> mScenes;
		SceneInfo* mCurrentSceneInfo = nullptr;
		size_t mNextSceneID = std::numeric_limits<size_t>::max();
		bool mIsPlaying = false;
		EventDispatcher<> mOnScenePreLoad;
		EventDispatcher<Scene&> mOnScenePostLoad;
		SceneSettings mSettings;
		std::weak_ptr<Blackboard> mBlackboard;
	};
}