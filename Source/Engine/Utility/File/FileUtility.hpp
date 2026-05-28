#pragma once
#include <External/nlohmann/json.hpp>
#include <vector>
#include <string>

namespace CLX
{
	class FileUtility final
	{
	public:
		static void CopyFiles(std::span<const std::filesystem::path> paths, const std::filesystem::path& dropPath);

		// Returns absolute paths
		static std::vector<std::filesystem::path> GetPathsFromDirectory(const std::filesystem::path& absolutePath, const bool includeFolders = false);
		static std::optional<nlohmann::json> GetDataAsJson(const std::filesystem::path& absolutePath);

		
	};

	inline std::filesystem::path GetAbsoluteAssetPath(const std::filesystem::path& relativePath)
	{
        return std::filesystem::absolute(SIMPLE_DIR_ASSETS / relativePath);
	}

	inline std::filesystem::path GetAbsoluteAssetPath()
	{
		return std::filesystem::absolute(SIMPLE_DIR_ASSETS);
	}

	inline std::filesystem::path GetAbsoluteSettingsPath()
	{
		return std::filesystem::absolute(std::filesystem::path(COMPLEX_DIR_SETTINGS));
	}

	inline std::filesystem::path GetAbsoluteProjectSettingsPath()
	{
		return std::filesystem::absolute(std::filesystem::path(COMPLEX_DIR_SETTINGS) / "project_settings.json");
	}

	inline std::filesystem::path GetAbsoluteGameSettingsPath()
	{
		return std::filesystem::absolute(std::filesystem::path(COMPLEX_DIR_SETTINGS) / "game_settings.json");
	}

	inline std::filesystem::path GetAbsoluteImGuiSettingsPath()
	{
		return std::filesystem::absolute(std::filesystem::path(COMPLEX_DIR_SETTINGS) / "imgui.ini");
	}
}