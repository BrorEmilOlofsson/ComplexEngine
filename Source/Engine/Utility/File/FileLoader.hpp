#pragma once
#include <filesystem>
#include "Engine/Utility/MeshData.hpp"

namespace Simple
{

	class FileLoader final
	{
	public:


		[[nodiscard]] static MeshData<Vertex> LoadObj(const std::filesystem::path& path);

	private:



	};
}