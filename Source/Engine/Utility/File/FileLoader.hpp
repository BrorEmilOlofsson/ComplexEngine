#pragma once
#include <filesystem>
#include "Engine/Utility/MeshData.hpp"

namespace CLX
{

	class FileLoader final
	{
	public:


		[[nodiscard]] static MeshData<Vertex> LoadObj(const std::filesystem::path& path);

	private:



	};
}