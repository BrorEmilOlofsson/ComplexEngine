#pragma once
#include <External/nlohmann/json.hpp>
#include <vector>
#include <string>

namespace Simple
{
	class FileUtility final
	{
	public:
		static void CopyFiles(std::span<const std::filesystem::path> paths, const std::filesystem::path& dropPath);

		static std::vector<std::filesystem::path> GetFilesFromDirectory(const std::filesystem::path& absolutePath, const bool includeFolders = false);
		static std::optional<nlohmann::json> GetDataAsJson(const std::filesystem::path& absolutePath);
	};
}