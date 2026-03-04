#include "Engine/Precompiled/EnginePch.hpp"
#include <fstream>
#include "FileLoader.hpp"

namespace Simple
{
	MeshData<Vertex> FileLoader::LoadObj(const std::filesystem::path& path)
	{
		PROFILER_FUNCTION(profiler::colors::RichBlue);
		MeshData<Vertex> meshData;
		std::ifstream meshFile(std::filesystem::absolute(path), std::ios_base::in);

		if (!meshFile.is_open() || meshFile.fail())
		{
			return MeshData<Vertex>();
		}

		while (!meshFile.eof())
		{
			std::string reader;
			meshFile >> reader;
			if (reader == "v")
			{
				float x, y, z;
				meshFile >> x;
				meshFile >> y;
				meshFile >> z;

				Vertex& vertex = meshData.vertices.emplace_back();
				vertex.position = Point3f(x, y, z);

			}
			else if (reader == "f")
			{
				int index1, index2, index3;

				meshFile >> index1;

				meshFile >> index2;

				meshFile >> index3;

				meshData.indices.push_back(index1 - 1);
				meshData.indices.push_back(index2 - 1);
				meshData.indices.push_back(index3 - 1);
			}
		}
		return meshData;
	}
}
