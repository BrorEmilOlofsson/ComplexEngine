#pragma once
#include <filesystem>
#include <expected>
#include "Engine/Utility/Blackboard.hpp"
#include "Scene.hpp"

namespace CLX
{

    enum class eSceneLoaderError
    {
        FileNotFound,
        FailedToLoadScene
    };

	class Scene;

	class SceneLoader
	{
	public:
		static void SaveScene(const Scene& scene, const std::filesystem::path& path, const Blackboard& blackboard);
		[[nodiscard]] static std::expected<Scene, eSceneLoaderError> LoadScene(RenderContext&& renderContext, const std::filesystem::path& path, std::weak_ptr<Blackboard> blackboardPtr);
		static void CreateNewSceneFile(const std::filesystem::path& path);
	};

}