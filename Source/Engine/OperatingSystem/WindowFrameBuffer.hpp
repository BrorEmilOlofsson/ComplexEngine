#pragma once
#include <cstdint>
#include <vector>
#include <filesystem>

namespace CLX
{

	enum class eQuit : uint8_t
	{
		Yes,
		No
	};

	struct WindowFrameBuffer final
	{
		bool hasResized = false;
		bool hasQuit = false;
		std::vector<std::filesystem::path> droppedFiles;
	};

}