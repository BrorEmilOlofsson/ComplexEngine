#include "Engine/Precompiled/EnginePch.hpp"
#include "SceneManager.hpp"
#include "Engine/Asset/AssetManager.hpp"

namespace CLX
{

    void SetActiveSceneAndRunSceneEvents(SceneAssetHandle& outScene, SceneAssetHandle newScene, auto&& postChangeEvents)
    {
        if (!newScene)
        {
            throw std::invalid_argument("Failed to set active scene! The provided scene asset handle is invalid.");
        }
        Scene& sceneRef = *newScene.Get();
        outScene = newScene;
        for (const auto& function : postChangeEvents)
        {
            function(sceneRef);
        }
    }

    SceneManager::SceneManager(std::weak_ptr<AssetManager> assetManager)
        : mAssetManager(std::move(assetManager))
    {
    }

    [[nodiscard]] SceneAssetHandle SceneManager::GetActiveScene() const noexcept
    {
        return mActiveScene;
    }

    void SceneManager::BeginPlay()
    {
        if (!mActiveScene)
        {
            return;
        }
        mActiveScene->BeginPlay();
    }

    void SceneManager::EndPlay()
    {
        if (!mActiveScene)
        {
            return;
        }
        mActiveScene->EndPlay();
    }

    void SceneManager::Update(float deltaTime)
    {
        if (!mActiveScene)
        {
            return;
        }

        if (mIsPlaying)
        {
            mActiveScene->Update(deltaTime);
        }
        else
        {
            mActiveScene->EditorUpdate();
        }
    }

    void SceneManager::BeginFrame(const Vector2ui windowSize, const Point2i mouseScreenPos)
    {
        if (!mActiveScene)
        {
            return;
        }

        if (mSceneToLoadNextFrame)
        {
            SetActiveSceneAndRunSceneEvents(mActiveScene, mSceneToLoadNextFrame, mOnSceneLoadedFunctions);
            mSceneToLoadNextFrame = SceneAssetHandle::Empty();
        }
        mActiveScene->BeginFrame(windowSize, mouseScreenPos);
    }

    void SceneManager::ChangeScene(const SceneAssetHandle sceneAsset)
    {
        mSceneToLoadNextFrame = sceneAsset;
    }

    void SceneManager::ChangeSceneDirectly(const SceneAssetHandle sceneAsset)
    {
        if (!sceneAsset)
        {
            throw std::invalid_argument("Failed to change scene! The provided scene asset handle is invalid.");
        }

        SetActiveSceneAndRunSceneEvents(mActiveScene, sceneAsset, mOnSceneLoadedFunctions);
    }

}