#pragma once
#include "Engine/Graphics/DX11/Mesh/DX11Mesh.hpp"

namespace Simple
{
	struct TerrainData
	{
		float amplitude = 5.0f;
		float vertexLength = 0.50f;
		unsigned int gridSize = 12;
		unsigned int upSampleMultiplier = 2;
	};
}

namespace Simple
{
	class ShapeCreator3000 final
	{
	public:
		static MeshData<Vertex> CreateTerrain(const TerrainData& terrainData = TerrainData{});
		static MeshData<Vertex> CreatePyramid(const Vector3f& size = Vector3f::One());
		static MeshData<Vertex> CreatePyramidNew();
		static MeshData<Vertex> CreateCone(const float radius = 0.5f, const float height = 1.f, Color color = Colors::Pink);
		static MeshData<Vertex> CreateCylinder(const float radius = 0.5f, const float height = 1.f, Color color = Colors::Pink);
		static MeshData<Vertex> CreateCube(const Vector3f& size = Vector3f(0.5f, 0.5f , 0.5f));
		static MeshData<Vertex> CreateSkyBox(const Vector3f& size = Vector3f(1000.0f, 1000.0f, 1000.0f));
		static MeshData<Vertex> CreatePlane();
		static MeshData<Vertex> Create2DPlane();
		static MeshData<Vertex> CreateSphere(const float radius = 1.0f, const int slices = 30, const int stacks = 30);
		static MeshData<Vertex> CreateArrow();
		static MeshData<Vertex> CreateQuad();
	};
}