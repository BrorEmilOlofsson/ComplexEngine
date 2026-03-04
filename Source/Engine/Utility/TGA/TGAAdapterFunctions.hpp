#pragma once
#include "Engine/Utility/MeshData.hpp"
#include <External/TheGameAssembly/FBXImporter/Source/Importer.h>

namespace CLX
{

	inline MeshData<Vertex> LoadMeshData(const TGA::FBX::Mesh& tgaMesh)
	{
		MeshData<Vertex> meshData;
		meshData.vertices.reserve(tgaMesh.Elements[0].Vertices.size());
		meshData.indices.reserve(tgaMesh.Elements[0].Indices.size());

		for (size_t i = 0; i < tgaMesh.Elements[0].Vertices.size(); ++i)
		{
			const TGA::FBX::Vertex& currentVertex = tgaMesh.Elements[0].Vertices[i];
			Vertex vertex;

			vertex.position.x = currentVertex.Position[0];
			vertex.position.y = currentVertex.Position[1];
			vertex.position.z = currentVertex.Position[2];
			vertex.w = currentVertex.Position[3];

			vertex.bones[0] = static_cast<uint32_t>(currentVertex.BoneIDs[0]);
			vertex.bones[1] = static_cast<uint32_t>(currentVertex.BoneIDs[1]);
			vertex.bones[2] = static_cast<uint32_t>(currentVertex.BoneIDs[2]);
			vertex.bones[3] = static_cast<uint32_t>(currentVertex.BoneIDs[3]);

			vertex.color.r = currentVertex.VertexColors[0][0];
			vertex.color.g = currentVertex.VertexColors[0][1];
			vertex.color.b = currentVertex.VertexColors[0][2];
			vertex.color.a = currentVertex.VertexColors[0][3];

			if (vertex.color.r + vertex.color.g + vertex.color.b + vertex.color.a == 0)
			{
				vertex.color = Colors::White;
			}

			vertex.weights[0] = currentVertex.BoneWeights[0];
			vertex.weights[1] = currentVertex.BoneWeights[1];
			vertex.weights[2] = currentVertex.BoneWeights[2];
			vertex.weights[3] = currentVertex.BoneWeights[3];

			vertex.normal = UnitVector3f(currentVertex.Normal[0], currentVertex.Normal[1], currentVertex.Normal[2]);
			vertex.tangent = UnitVector3f(currentVertex.Tangent[0], currentVertex.Tangent[1], currentVertex.Tangent[2]);
			vertex.bitangent = UnitVector3f(currentVertex.BiNormal[0], currentVertex.BiNormal[1], currentVertex.BiNormal[2]);

			vertex.uv.x = currentVertex.UVs[0][0];
			vertex.uv.y = currentVertex.UVs[0][1];

			meshData.vertices.push_back(vertex);
		}

		meshData.indices = tgaMesh.Elements[0].Indices;
		return meshData;
	}
}