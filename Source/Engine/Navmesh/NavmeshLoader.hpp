#pragma once
#include "NavmeshData.hpp"

#include <filesystem>

namespace Simple
{
	class NavmeshLoader
	{
	public:

		static NavmeshData LoadMesh(const std::filesystem::path& aObjFilePath);
	};
}
