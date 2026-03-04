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
}