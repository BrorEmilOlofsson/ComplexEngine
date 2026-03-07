#pragma once
#include "Engine/Asset/AssetTypes/SceneAsset.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Scene/SceneSettings.hpp"

namespace CLX
{

	class AssetManager;

	class SceneManager final
	{
	public:

		explicit SceneManager(std::weak_ptr<AssetManager> assetManager);

		[[nodiscard]] SceneAssetHandle GetActiveScene() const noexcept;
		[[nodiscard]] const SceneSettings& GetSceneSettings() const noexcept { return mSceneSettings; }
        [[nodiscard]] SceneSettings& GetSceneSettings() noexcept { return mSceneSettings; }

        void BeginPlay();
		void EndPlay();

        void Update(float deltaTime);
		void BeginFrame(Vector2ui windowSize, Point2i mouseScreenPos);

		void ChangeScene(SceneAssetHandle scene);
		void ChangeSceneDirectly(SceneAssetHandle scene);

        [[nodiscard]] bool IsPlaying() const noexcept { return mIsPlaying; }
        void SetIsPlaying(bool isPlaying) noexcept { mIsPlaying = isPlaying; }
		void RegisterOnSceneLoadedFunction(std::function<void(Scene&)> function)
		{
			mOnSceneLoadedFunctions.push_back(std::move(function));
		}

	private:

        SceneAssetHandle mActiveScene;

		std::weak_ptr<AssetManager> mAssetManager;

        std::vector<std::function<void(Scene&)>> mOnSceneLoadedFunctions;

        SceneSettings mSceneSettings;
        bool mIsPlaying = false;
		SceneAssetHandle mSceneToLoadNextFrame;
	};
}