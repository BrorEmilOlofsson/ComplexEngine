#include "Utility/Precompiled/UtilityPch.hpp"
#include "FileLoader.hpp"
#include <fstream>

namespace Simple
{
	MeshData<Vertex> FileLoader::LoadObj(const std::filesystem::path& path)
	{
		PROFILER_FUNCTION(profiler::colors::RichBlue);
		MeshData<Vertex> meshData;
		std::fstream meshLoader(std::filesystem::absolute(path), std::ios_base::in);
		assert(meshLoader.is_open() && !meshLoader.fail() && "Error! Couldn't open file!");
		while (!meshLoader.eof())
		{
			std::string reader;
			meshLoader >> reader;
			if (reader == "v")
			{
				float x, y, z;
				meshLoader >> x;
				meshLoader >> y;
				meshLoader >> z;

				Vertex& vertex = meshData.vertices.emplace_back();
				vertex.position = Point3f(x, y, z);

			}
			else if (reader == "f")
			{
				int index1, index2, index3;

				meshLoader >> index1;

				meshLoader >> index2;

				meshLoader >> index3;

				meshData.indices.push_back(index1 - 1);
				meshData.indices.push_back(index2 - 1);
				meshData.indices.push_back(index3 - 1);
			}
		}
		return meshData;
	}
}
