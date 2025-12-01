#include "Utility/Precompiled/UtilityPch.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Utility/Win/WinConsole.hpp"
#include <fstream>

namespace Simple
{

	void FileUtility::CopyFiles(std::span<const std::filesystem::path> paths, const std::filesystem::path& dropPath)
	{
		for (const auto& path : paths)
		{
			const std::filesystem::path destinationPath = dropPath / path.filename();

			/*if (std::filesystem::is_directory(path))
			{
				Console::Print("Path ", ConsoleTextColor::White, false);
				Console::Print(path.string(), ConsoleTextColor::Yellow, false);
				Console::Print(" is a folder and will be skipped.\n", ConsoleTextColor::White, true);
			}

			std::error_code code;*/
			std::filesystem::copy_options copyOptions = std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing;

			if (std::filesystem::copy_file(path, destinationPath.string().c_str(), copyOptions))
			{
				Console::Print("File ", ConsoleTextColor::White, false);
				Console::Print(path.string(), ConsoleTextColor::Green, false);
				Console::Print(" has been copied to ", ConsoleTextColor::White, false);
				Console::Print(dropPath.string(), ConsoleTextColor::Green, true);
			}
			else
			{
				Console::Print("File ", ConsoleTextColor::White, false);
				Console::Print(path.string(), ConsoleTextColor::Red, false);
				Console::Print(" already exist at ", ConsoleTextColor::White, false);
				Console::Print(dropPath.string(), ConsoleTextColor::Red, true);
			}

		}

	}

	std::vector<std::filesystem::path> FileUtility::GetPathsFromDirectory(const std::filesystem::path& absolutePath, const bool includeFolders)
	{
		PROFILER_FUNCTION(profiler::colors::Brown);
		std::vector<std::filesystem::path> files;

		for (const auto& entry : std::filesystem::directory_iterator(absolutePath))
		{
			if (!includeFolders && !entry.is_regular_file())
			{
				continue;
			}

			files.push_back(entry.path());
		}

		return files;
	}

	std::optional<nlohmann::json> FileUtility::GetDataAsJson(const std::filesystem::path& absolutePath)
	{
		std::ifstream file(absolutePath);

		if (!file.is_open())
		{
			return std::nullopt;
		}

		return nlohmann::json::parse(file);
	}
}