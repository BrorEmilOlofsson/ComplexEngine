#pragma once
#include <vector>
#include <filesystem>

namespace CLX
{

	enum class eQuit : unsigned char
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