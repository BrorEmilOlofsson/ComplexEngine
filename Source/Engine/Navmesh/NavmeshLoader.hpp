#pragma once
#include "NavmeshData.hpp"

#include <filesystem>

namespace CLX
{
	class NavmeshLoader
	{
	public:

		static NavmeshData LoadMesh(const std::filesystem::path& objFilePath);
	};
}
