#pragma once
#include "Utility/Algorithm.hpp"
#include "External/profiler.h"

namespace Simple
{
	std::filesystem::path InsertBeforeExtension(const std::filesystem::path& aPath, const std::filesystem::path& aExtension, const std::string& aText)
	{
		return aPath.parent_path() / std::filesystem::path(aPath.stem().string() + aText + aExtension.string());
	}

	void EasyProfilerOutput()
	{
		std::filesystem::path filePath = std::filesystem::path(SIMPLE_FILENAME_PROFILER);
		const std::filesystem::path extension = filePath.extension();

#ifdef _DEBUG_EDITOR
		filePath = InsertBeforeExtension(filePath, extension, "_Debug_Editor");
#elif _RELEASE_EDITOR
		filePath = InsertBeforeExtension(filePath, extension, "_Release_Editor");
#elif _DEBUG_GAME
		filePath = InsertBeforeExtension(filePath, extension, "_Debug_Game");
#elif _RELEASE_GAME
		filePath = InsertBeforeExtension(filePath, extension, "_Release_Game");
#endif

		const std::filesystem::path fileNameWithCounter = AppendCounterIfAlreadyExist(filePath);

		PROFILER_DUMP_FILE(std::filesystem::absolute(fileNameWithCounter).string().c_str());
	}
}