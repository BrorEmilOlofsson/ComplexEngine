#pragma once
#include <string>
#include <filesystem>

namespace CLX
{

	class Scene;

	class SceneLoader
	{
	public:
		static void SaveScene(const Scene& scene, const std::filesystem::path& fileName, const class DataTypeRegistry& dataTypeRegistry);
		static void LoadScene(Scene& scene, const std::filesystem::path& fileName, const Blackboard& blackboard);
		static void CreateNewSceneFile(const std::filesystem::path& fileName);
	};

}