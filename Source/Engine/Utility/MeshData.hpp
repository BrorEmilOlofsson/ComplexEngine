#pragma once
#include <vector>
#include "Engine/Utility/Vertex.hpp"

namespace CLX
{
	template<typename VertexType>
	struct MeshData final
	{
		std::vector<VertexType> vertices;
		std::vector<unsigned int> indices;
	};
}