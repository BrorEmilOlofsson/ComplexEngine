#pragma once
#include <filesystem>
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

	class Scene;

	class SceneLoader
	{
	public:
		static void SaveScene(const Scene& scene, const std::filesystem::path& path, const Blackboard& blackboard);
		static void LoadScene(Scene& scene, const std::filesystem::path& path, const Blackboard& blackboard);
		static void CreateNewSceneFile(const std::filesystem::path& path);
	};

}