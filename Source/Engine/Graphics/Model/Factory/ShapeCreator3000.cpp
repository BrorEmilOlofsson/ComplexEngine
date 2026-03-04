#include "Engine/Precompiled/EnginePch.hpp"
#include "ShapeCreator3000.hpp"
#include <External/TheGameAssembly/Uppgift5/uppgift05_helper.h>

namespace CLX
{
	MeshData<Vertex> ShapeCreator3000::CreateTerrain(const TerrainData& terrainData)
	{
		const unsigned int upSampleMultiplier = terrainData.upSampleMultiplier;
		const float vertexLength = terrainData.vertexLength;
		const float amplitude = terrainData.amplitude;

		unsigned int gridSize = terrainData.gridSize;

		std::vector<float> heightMap(gridSize * gridSize);

		{
			float amplitudeMultiplier = amplitude;
			for (unsigned int i = 0; i < upSampleMultiplier; ++i)
			{
				Tga::AddNoise(heightMap, amplitudeMultiplier);

				heightMap = Tga::Upsample2X(heightMap, gridSize);

				amplitudeMultiplier *= 0.25f;
				gridSize *= 2;
			}
		}

		std::vector<Vertex> vertices(gridSize * gridSize);
		std::vector<unsigned int> indices;

		for (unsigned int x = 0; x < gridSize; ++x)
		{
			for (unsigned int y = 0; y < gridSize; ++y)
			{
				const unsigned int index = x + (gridSize * y);

				const Point3f position = Point3f::Zero() + Vector3f(static_cast<float>(x), heightMap[index], static_cast<float>(y)) * vertexLength;
				const float u = static_cast<float>(x) / gridSize;
				const float v = static_cast<float>(y) / gridSize;

				vertices[index].position = position;
				vertices[index].normal = UnitVector3f::Up();
				vertices[index].uv = Vector2f(u, v);
			}
		}

		for (unsigned int x = 0; x < gridSize - 1; ++x)
		{
			for (unsigned int y = 0; y < gridSize - 1; ++y)
			{
				const unsigned int index = x + (gridSize * y);

				const unsigned int north = index + gridSize;
				const unsigned int northEast = north + 1;
				const unsigned int east = index + 1;

				// Clockwise - very important!!!
				indices.push_back(index);
				indices.push_back(north);
				indices.push_back(northEast);

				indices.push_back(index);
				indices.push_back(northEast);
				indices.push_back(east);
			}
		}

		for (unsigned int x = 0; x < gridSize; ++x)
		{
			for (unsigned int y = 0; y < gridSize; ++y)
			{
				const unsigned int index = x + (gridSize * y);

				unsigned int north = index + gridSize;
				unsigned int south = index - gridSize;
				unsigned int east = index + 1;
				unsigned int west = index - 1;

				if (y == 0)
				{
					south = index;
				}
				else if (y == (gridSize - 1))
				{
					north = index;
				}

				if (x == 0)
				{
					west = index;
				}
				else if (x == (gridSize - 1))
				{
					east = index;
				}

				Vector3f tangentVertical = vertices[north].position - vertices[south].position;
				Vector3f tangentHorizontal = vertices[west].position - vertices[east].position;
				UnitVector3f normal = UnitVector3f(Cross(tangentHorizontal, tangentVertical));

				vertices[index].normal = normal;
				vertices[index].tangent = Cross(vertices[index].normal, UnitVector3f::Forward());
				vertices[index].bitangent = Cross(vertices[index].normal, vertices[index].tangent);
			}
		}

		return MeshData(vertices, indices);
	}

	static UnitVector3f CalculatePyramidNormal(const Point3f& vertexA, const Point3f& vertexB, const Point3f& vertexC)
	{
		const Vector3f BA = vertexB - vertexA;
		const Vector3f CB = vertexC - vertexB;
		const UnitVector3f normal = UnitVector3f(Cross(BA, CB));

		return normal;
	}

	MeshData<Vertex> ShapeCreator3000::CreatePyramid(const Vector3f& size)
	{
		UnitVector3f normal;

		Vertex south_West;
		south_West.position = Point3f(-size.x, -size.y, -size.z);
		south_West.uv = Vector2f(0.0f, 0.0f);

		Vertex south_East;
		south_East.position = Point3f(size.x, -size.y, -size.z);
		south_East.uv = Vector2f(1.0f, 0.0f);

		Vertex south_Top;
		south_Top.position = Point3f(0.0f, size.y, 0.0f);
		south_Top.uv = Vector2f(0.6f, 1.0f);

		normal = CalculatePyramidNormal(south_West.position, south_Top.position, south_East.position);
		south_West.normal = normal;
		south_East.normal = normal;
		south_Top.normal = normal;

		Vertex east_West;
		east_West.position = Point3f(size.x, -size.y, -size.z);
		east_West.uv = Vector2f(0.0f, 0.0f);

		Vertex east_East;
		east_East.position = Point3f(size.x, -size.y, size.z);
		east_East.uv = Vector2f(1.0f, 0.0f);

		Vertex east_Top;
		east_Top.position = Point3f(0.0f, size.y, 0.0f);
		east_Top.uv = Vector2f(0.6f, 1.0f);
		east_Top.normal = UnitVector3f::Right();

		normal = CalculatePyramidNormal(east_West.position, east_Top.position, east_East.position);
		east_West.normal = normal;
		east_East.normal = normal;
		east_Top.normal = normal;

		Vertex north_West;
		north_West.position = Point3f(-size.x, -size.y, size.z);
		north_West.uv = Vector2f(0.0f, 0.0f);

		Vertex north_East;
		north_East.position = Point3f(size.x, -size.y, size.z);
		north_East.uv = Vector2f(1.0f, 0.0f);

		Vertex north_Top;
		north_Top.position = Point3f(0.0f, size.y, 0.0f);
		north_Top.uv = Vector2f(0.6f, 1.0f);

		normal = CalculatePyramidNormal(north_East.position, north_Top.position, north_West.position);
		north_West.normal = normal;
		north_East.normal = normal;
		north_Top.normal = normal;

		Vertex west_West;
		west_West.position = Point3f(-size.x, -size.y, size.z);
		west_West.uv = Vector2f(0.0f, 0.0f);

		Vertex west_East;
		west_East.position = Point3f(-size.x, -size.y, -size.z);
		west_East.uv = Vector2f(1.0f, 0.0f);

		Vertex west_Top;
		west_Top.position = Point3f(0.0f, size.y, 0.0f);
		west_Top.uv = Vector2f(0.6f, 1.0f);

		normal = CalculatePyramidNormal(west_West.position, west_Top.position, west_East.position);
		west_West.normal = normal;
		west_East.normal = normal;
		west_Top.normal = normal;

		Vertex southWest;
		southWest.position = south_West.position;
		southWest.uv = Vector2f(0.0f, 0.0f);
		southWest.normal = UnitVector3f::Down();

		Vertex northWest;
		northWest.position = west_West.position;
		northWest.uv = Vector2f(0.0f, 1.0f);
		northWest.normal = UnitVector3f::Down();

		Vertex northEast;
		northEast.position = east_East.position;
		northEast.uv = Vector2f(1.0f, 1.0f);
		northEast.normal = UnitVector3f::Down();

		Vertex southEast;
		southEast.position = south_East.position;
		southEast.uv = Vector2f(1.0f, 0.0f);
		southEast.normal = UnitVector3f::Down();

		std::vector<Vertex> vertices =
		{
			south_West, south_Top, south_East,
			east_West, east_Top, east_East,
			north_East, north_Top, north_West,
			west_West, west_Top, west_East,
			southWest, northWest, northEast, southEast
		};

		std::vector<unsigned int> indices =
		{
			//Base
			14,13,12,
			15,14,12,

			//Sides
			0,9, 1,
			0,1,2,
			3,4,5,
			6,7,8,
			9,10,11
		};

		return MeshData(vertices, indices);
	}

	MeshData<Vertex> ShapeCreator3000::CreatePyramidNew()
	{
		MeshData<Vertex> meshData;

		Vertex& bottomLeft = meshData.vertices.emplace_back();
		bottomLeft.position = Point3f(-0.5f, 0, -0.5f);

		Vertex& topLeft = meshData.vertices.emplace_back();
		topLeft.position = Point3f(-0.5f, 0, 0.5f);

		Vertex& topRight = meshData.vertices.emplace_back();
		topRight.position = Point3f(0.5f, 0, 0.5f);

		Vertex& bottomRight = meshData.vertices.emplace_back();
		bottomRight.position = Point3f(0.5f, 0, -0.5f);

		Vertex& top = meshData.vertices.emplace_back();
		top.position = Point3f(0, 1, 0);

		meshData.indices =
		{
			0, 1, 2,
			0, 2, 3,
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};

		return meshData;
	}

	MeshData<Vertex> ShapeCreator3000::CreateCone(const float radius, const float height, Color color)
	{
		MeshData<Vertex> meshData;

		constexpr size_t triangleCount = 20;

		meshData.vertices.reserve(triangleCount + 2);


		const Point3f startPoint(radius, 0, 0);

		for (size_t i = 0; i < triangleCount; i++)
		{
			Vertex& vertex = meshData.vertices.emplace_back();
			const Radiansf currentAngle = (WholeRotationRad / static_cast<float>(triangleCount)) * static_cast<float>(i);
			vertex.position = RotatePointAroundAxis(Point3f::Zero(), startPoint, UnitVector3f::Up(), currentAngle);
			vertex.color = color;
			vertex.normal = GetUnitVector(Point3f::Zero(), vertex.position);
		}

		{
			Vertex& topVertex = meshData.vertices.emplace_back();
			topVertex.position = Point3f(0, height, 0);
			topVertex.color = color;
			topVertex.normal = UnitVector3f::Up();
		}

		{
			Vertex& bottomVertex = meshData.vertices.emplace_back();
			bottomVertex.position = Point3f(0, 0, 0);
			bottomVertex.color = color;
			bottomVertex.normal = UnitVector3f::Down();
		}

		for (unsigned int i = 0; i < triangleCount; i++)
		{
			meshData.indices.push_back(i);
			meshData.indices.push_back((static_cast<unsigned int>(i) + 1) % static_cast<unsigned int>(triangleCount));
			meshData.indices.push_back(static_cast<unsigned int>(meshData.vertices.size()) - 2);
		}

		for (unsigned int i = 0; i < triangleCount; i++)
		{
			meshData.indices.push_back(i);
			meshData.indices.push_back(static_cast<unsigned int>(meshData.vertices.size()) - 1);
			meshData.indices.push_back((static_cast<unsigned int>(i) + 1) % static_cast<unsigned int>(triangleCount));
		}

		return meshData;
	}

	static std::vector<Vertex> CreateDiskVertices(const std::size_t aVertexCount, const float radius, Point3f aCenterPoint, const UnitVector3f& aRotationAxis, Color color)
	{
		std::vector<Vertex> vertices;

		vertices.reserve(aVertexCount);


		const Point3f startPoint = aCenterPoint + GetPerpendicularVector(aRotationAxis) * radius;

		for (size_t i = 0; i < aVertexCount; i++)
		{
			Vertex& vertex = vertices.emplace_back();
			const Radiansf currentAngle = (WholeRotationRad / static_cast<float>(aVertexCount)) * static_cast<float>(i);
			vertex.position = RotatePointAroundAxis(aCenterPoint, startPoint, aRotationAxis, currentAngle);
			vertex.color = color;
		}

		return vertices;

	}

	MeshData<Vertex> ShapeCreator3000::CreateCylinder(const float radius, const float height, Color color)
	{
		MeshData<Vertex> meshData;
		const Point3f middle = Point3f::Zero() + Vector3f(0, height / 2, 0);
		constexpr size_t rotationCount = 20;

		meshData.vertices.reserve(rotationCount * 2 + 2);

		{
			auto bottomDiskVertices = CreateDiskVertices(rotationCount, radius, Point3f::Zero(), UnitVector3f::Up(), color);
			for (auto& vertex : bottomDiskVertices)
			{
				vertex.normal = GetUnitVector(middle, vertex.position);
			}
			meshData.vertices.insert(end(meshData.vertices), begin(bottomDiskVertices), end(bottomDiskVertices));
		}
		{
			Vertex& bottomVertex = meshData.vertices.emplace_back();
			bottomVertex.position = Point3f::Zero();
			bottomVertex.color = color;
			bottomVertex.normal = UnitVector3f::Down();
		}
		{
			auto topDiskVertices = CreateDiskVertices(rotationCount, radius, Point3f(0, height, 0), UnitVector3f::Up(), color);
			for (auto& vertex : topDiskVertices)
			{
				vertex.normal = GetUnitVector(middle, vertex.position);
			}
			meshData.vertices.insert(end(meshData.vertices), begin(topDiskVertices), end(topDiskVertices));
		}
		{
			Vertex& topVertex = meshData.vertices.emplace_back();
			topVertex.position = Point3f(0, height, 0);
			topVertex.color = color;
			topVertex.normal = UnitVector3f::Up();
		}


		for (unsigned int i = 0; i < rotationCount; i++)
		{
			meshData.indices.push_back(i);
			meshData.indices.push_back(static_cast<unsigned int>(rotationCount));
			meshData.indices.push_back((static_cast<unsigned int>(i) + 1) % static_cast<unsigned int>(rotationCount));
		}

		{
			const unsigned int start = rotationCount + 1;
			for (unsigned int i = start; i < start + rotationCount; i++)
			{
				meshData.indices.push_back(i);
				meshData.indices.push_back(Wrap<unsigned int>(i + 1, start, start + rotationCount - 1));
				meshData.indices.push_back(static_cast<unsigned int>(meshData.vertices.size() - 1));
			}
		}

		//// Sides
		for (unsigned int i = 0; i < rotationCount; i++)
		{
			const unsigned int bottomRight = i;
			const unsigned int bottomLeft = Wrap<unsigned int>(i + 1, 0, rotationCount - 1);
			const unsigned int topRight = i + rotationCount + 1;
			const unsigned int topLeft = Wrap<unsigned int>(topRight + 1, rotationCount + 1, rotationCount * 2);

			meshData.indices.push_back(bottomRight);
			meshData.indices.push_back(bottomLeft);
			meshData.indices.push_back(topRight);

			meshData.indices.push_back(bottomLeft);
			meshData.indices.push_back(topLeft);
			meshData.indices.push_back(topRight);
		}

		return meshData;
	}

	MeshData<Vertex> ShapeCreator3000::CreateCube(const Vector3f& size)
	{
		Vertex north_bottomLeft;
		north_bottomLeft.position = Point3f(size.x, -size.y, size.z);
		north_bottomLeft.uv = Vector2f::Zero();
		north_bottomLeft.normal = UnitVector3f::Forward();
		north_bottomLeft.tangent = UnitVector3f::Right();
		north_bottomLeft.bitangent = UnitVector3f::Up();

		Vertex north_bottomRight;
		north_bottomRight.position = Point3f(-size.x, -size.y, size.z);
		north_bottomRight.uv = Vector2f(1, 0);
		north_bottomRight.normal = UnitVector3f::Forward();
		north_bottomRight.tangent = UnitVector3f::Right();
		north_bottomRight.bitangent = UnitVector3f::Up();

		Vertex north_topLeft;
		north_topLeft.position = Point3f(size.x, size.y, size.z);
		north_topLeft.uv = Vector2f(0, 1);
		north_topLeft.normal = UnitVector3f::Forward();
		north_topLeft.tangent = UnitVector3f::Right();
		north_topLeft.bitangent = UnitVector3f::Up();

		Vertex north_topRight;
		north_topRight.position = Point3f(-size.x, size.y, size.z);
		north_topRight.uv = Vector2f::One();
		north_topRight.normal = UnitVector3f::Forward();
		north_topRight.tangent = UnitVector3f::Right();
		north_topRight.bitangent = UnitVector3f::Up();

		Vertex west_bottomLeft;
		west_bottomLeft.position = Point3f(-size.x, -size.y, size.z);
		west_bottomLeft.uv = Vector2f::Zero();
		west_bottomLeft.normal = UnitVector3f::Left();
		west_bottomLeft.tangent = UnitVector3f::Forward();
		west_bottomLeft.bitangent = UnitVector3f::Up();

		Vertex west_bottomRight;
		west_bottomRight.position = Point3f(-size.x, -size.y, -size.z);
		west_bottomRight.uv = Vector2f(1, 0);
		west_bottomRight.normal = UnitVector3f::Left();
		west_bottomRight.tangent = UnitVector3f::Forward();
		west_bottomRight.bitangent = UnitVector3f::Up();

		Vertex west_topLeft;
		west_topLeft.position = Point3f(-size.x, size.y, size.z);
		west_topLeft.uv = Vector2f(0, 1);
		west_topLeft.normal = UnitVector3f::Left();
		west_topLeft.tangent = UnitVector3f::Forward();
		west_topLeft.bitangent = UnitVector3f::Up();

		Vertex west_topRight;
		west_topRight.position = Point3f(-size.x, size.y, -size.z);
		west_topRight.uv = Vector2f::One();
		west_topRight.normal = UnitVector3f::Left();
		west_topRight.tangent = UnitVector3f::Forward();
		west_topRight.bitangent = UnitVector3f::Up();

		Vertex south_bottomLeft;
		south_bottomLeft.position = Point3f(-size.x, -size.y, -size.z);
		south_bottomLeft.uv = Vector2f::Zero();
		south_bottomLeft.normal = UnitVector3f::Backward();
		south_bottomLeft.tangent = UnitVector3f::Right();
		south_bottomLeft.bitangent = UnitVector3f::Up();

		Vertex south_bottomRight;
		south_bottomRight.position = Point3f(size.x, -size.y, -size.z);
		south_bottomRight.uv = Vector2f(1, 0);
		south_bottomRight.normal = UnitVector3f::Backward();
		south_bottomRight.tangent = UnitVector3f::Right();
		south_bottomRight.bitangent = UnitVector3f::Up();

		Vertex south_topLeft;
		south_topLeft.position = Point3f(-size.x, size.y, -size.z);
		south_topLeft.uv = Vector2f(0, 1);
		south_topLeft.normal = UnitVector3f::Backward();
		south_topLeft.tangent = UnitVector3f::Right();
		south_topLeft.bitangent = UnitVector3f::Up();

		Vertex south_topRight;
		south_topRight.position = Point3f(size.x, size.y, -size.z);
		south_topRight.uv = Vector2f::One();
		south_topRight.normal = UnitVector3f::Backward();
		south_topRight.tangent = UnitVector3f::Right();
		south_topRight.bitangent = UnitVector3f::Up();

		Vertex east_bottomLeft;
		east_bottomLeft.position = Point3f(size.x, -size.y, -size.z);
		east_bottomLeft.uv = Vector2f::Zero();
		east_bottomLeft.normal = UnitVector3f::Right();
		east_bottomLeft.tangent = UnitVector3f::Backward();
		east_bottomLeft.bitangent = UnitVector3f::Up();

		Vertex east_bottomRight;
		east_bottomRight.position = Point3f(size.x, -size.y, size.z);
		east_bottomRight.uv = Vector2f(1, 0);
		east_bottomRight.normal = UnitVector3f::Right();
		east_bottomRight.tangent = UnitVector3f::Backward();
		east_bottomRight.bitangent = UnitVector3f::Up();

		Vertex east_topLeft;
		east_topLeft.position = Point3f(size.x, size.y, -size.z);
		east_topLeft.uv = Vector2f(0, 1);
		east_topLeft.normal = UnitVector3f::Right();
		east_topLeft.tangent = UnitVector3f::Backward();
		east_topLeft.bitangent = UnitVector3f::Up();

		Vertex east_topRight;
		east_topRight.position = Point3f(size.x, size.y, size.z);
		east_topRight.uv = Vector2f::One();
		east_topRight.normal = UnitVector3f::Right();
		east_topRight.tangent = UnitVector3f::Backward();
		east_topRight.bitangent = UnitVector3f::Up();

		Vertex up_bottomLeft;
		up_bottomLeft.position = Point3f(size.x, size.y, size.z);
		up_bottomLeft.uv = Vector2f::Zero();
		up_bottomLeft.normal = UnitVector3f::Up();
		up_bottomLeft.tangent = UnitVector3f::Right();
		up_bottomLeft.bitangent = UnitVector3f::Backward();

		Vertex up_bottomRight;
		up_bottomRight.position = Point3f(-size.x, size.y, size.z);
		up_bottomRight.uv = Vector2f(1, 0);
		up_bottomRight.normal = UnitVector3f::Up();
		up_bottomRight.tangent = UnitVector3f::Right();
		up_bottomRight.bitangent = UnitVector3f::Backward();

		Vertex up_topLeft;
		up_topLeft.position = Point3f(size.x, size.y, -size.z);
		up_topLeft.uv = Vector2f(0, 1);
		up_topLeft.normal = UnitVector3f::Up();
		up_topLeft.tangent = UnitVector3f::Right();
		up_topLeft.bitangent = UnitVector3f::Backward();

		Vertex up_topRight;
		up_topRight.position = Point3f(-size.x, size.y, -size.z);
		up_topRight.uv = Vector2f::One();
		up_topRight.normal = UnitVector3f::Up();
		up_topRight.tangent = UnitVector3f::Right();
		up_topRight.bitangent = UnitVector3f::Backward();

		Vertex down_bottomLeft;
		down_bottomLeft.position = Point3f(-size.x, -size.y, size.z);
		down_bottomLeft.uv = Vector2f::Zero();
		down_bottomLeft.normal = UnitVector3f::Down();
		down_bottomLeft.tangent = UnitVector3f::Right();
		down_bottomLeft.bitangent = UnitVector3f::Forward();

		Vertex down_bottomRight;
		down_bottomRight.position = Point3f(size.x, -size.y, size.z);
		down_bottomRight.uv = Vector2f(1, 0);
		down_bottomRight.normal = UnitVector3f::Down();
		down_bottomRight.tangent = UnitVector3f::Right();
		down_bottomRight.bitangent = UnitVector3f::Forward();

		Vertex down_topLeft;
		down_topLeft.position = Point3f(-size.x, -size.y, -size.z);
		down_topLeft.uv = Vector2f(0, 1);
		down_topLeft.normal = UnitVector3f::Down();
		down_topLeft.tangent = UnitVector3f::Right();
		down_topLeft.bitangent = UnitVector3f::Forward();

		Vertex down_topRight;
		down_topRight.position = Point3f(size.x, -size.y, -size.z);
		down_topRight.uv = Vector2f::One();
		down_topRight.normal = UnitVector3f::Down();
		down_topRight.tangent = UnitVector3f::Right();
		down_topRight.bitangent = UnitVector3f::Forward();

		std::vector<Vertex> vertices =
		{
			north_bottomLeft, north_topLeft, north_topRight, north_bottomRight,
			west_bottomLeft, west_topLeft, west_topRight, west_bottomRight,
			south_bottomLeft, south_topLeft, south_topRight, south_bottomRight,
			east_bottomLeft, east_topLeft, east_topRight, east_bottomRight,
			up_bottomLeft, up_topLeft, up_topRight, up_bottomRight,
			down_bottomLeft, down_topLeft, down_topRight, down_bottomRight
		};

		std::vector<unsigned int> indices =
		{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11,
			12, 13, 14,
			12, 14, 15,
			16, 17, 18,
			16, 18, 19,
			20, 21, 22,
			20, 22, 23
		};

		return MeshData(vertices, indices);
	}

	MeshData<Vertex> ShapeCreator3000::CreatePlane()
	{
		Vertex southWest;
		southWest.position = Point3f(-1.0f, 0.0f, -1.0f);
		southWest.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		southWest.uv = Vector2f(0.0f, 0.0f);
		southWest.normal = UnitVector3f::Up();
		southWest.bitangent = UnitVector3f::Up();
		southWest.tangent = UnitVector3f::Up();
		southWest.color = Colors::Aqua;

		Vertex southEast;
		southEast.position = Point3f(1.0f, 0.0f, -1.0f);
		southEast.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		southEast.uv = Vector2f(1.0f, 0.0f);
		southEast.normal = UnitVector3f::Up();
		southEast.color = Colors::Aqua;

		Vertex northWest;
		northWest.position = Point3f(-1.0f, 0.0f, 1.0f);
		northWest.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		northWest.uv = Vector2f(0.0f, 1.0f);
		northWest.normal = UnitVector3f::Up();
		northWest.color = Colors::Aqua;

		Vertex northEast;
		northEast.position = Point3f(1.0f, 0.0f, 1.0f);
		northEast.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		northEast.uv = Vector2f(1.0f, 1.0f);
		northEast.normal = UnitVector3f::Up();
		northEast.color = Colors::Aqua;

		std::vector<Vertex> vertices =
		{
			southWest, southEast, northWest, northEast
		};

		std::vector<unsigned int> indices =
		{
			0, 2, 3,
			0, 3, 1
		};

		return MeshData(vertices, indices);
	}

	MeshData<Vertex> ShapeCreator3000::Create2DPlane()
	{
		Vertex southWest;
		southWest.position = Point3f(0.0f, 0.0f, 0.0f);
		southWest.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		southWest.uv = Vector2f(0.0f, 0.0f);
		southWest.normal = { 0.0f,0.0f,-1.0f };

		Vertex southEast;
		southEast.position = Point3f(1.0f, 0.0f, 0.0f);
		southEast.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		southEast.uv = Vector2f(1.0f, 0.0f);
		southEast.normal = { 0.0f,0.0f,-1.0f };

		Vertex northWest;
		northWest.position = Point3f(0.0f, 1.0f, 0.0f);
		northWest.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		northWest.uv = Vector2f(0.0f, 1.0f);
		northWest.normal = { 0.0f,0.0f,-1.0f };

		Vertex northEast;
		northEast.position = Point3f(1.0f, 1.0f, 0.0f);
		northEast.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		northEast.uv = Vector2f(1.0f, 1.0f);
		northEast.normal = { 0.0f,0.0f,-1.0f };

		std::vector<Vertex> vertices =
		{
			southWest, southEast, northWest, northEast
		};

		std::vector<unsigned int> indices =
		{
			0, 2, 3,
			0, 3, 1
		};

		return MeshData(vertices, indices);
	}

	MeshData<Vertex> ShapeCreator3000::CreateSphere(const float radius, const int slices, const int stacks)
	{
		std::vector<Vertex> vertices;

		for (int i = 0; i <= stacks; ++i)
		{
			const float phi = static_cast<float>(i) / stacks * PI<float>;
			for (int j = 0; j <= slices; ++j)
			{
				const float theta = static_cast<float>(j) * 2.0f * PI<float> / slices;

				Vertex vertex;
				vertex.position.x = radius * std::sin(phi) * std::cos(theta);
				vertex.position.y = radius * std::cos(phi);
				vertex.position.z = radius * std::sin(phi) * std::sin(theta);
				vertex.w = 1.0f;

				vertex.color = Colors::White;
				vertex.uv = Vector2f((4.0f + theta) / (2.0f * PI<float>), 1.0f - phi / PI<float>);
				vertex.normal = UnitVector3f(vertex.position - Point3f::Zero());

				vertices.push_back(vertex);
			}
		}

		std::vector<unsigned int> indices;

		for (int i = 0; i < stacks; ++i)
		{
			for (int j = 0; j < slices; ++j)
			{
				int v1 = i * (slices + 1) + j;
				int v2 = v1 + 1;
				int v3 = (i + 1) * (slices + 1) + j;
				int v4 = v3 + 1;

				indices.push_back(v2);
				indices.push_back(v3);
				indices.push_back(v1);

				indices.push_back(v4);
				indices.push_back(v3);
				indices.push_back(v2);
			}
		}

		return MeshData(vertices, indices);
	}

	MeshData<Vertex> ShapeCreator3000::CreateArrow()
	{
		MeshData<Vertex> meshData;

		std::vector<Vertex>& vertices = meshData.vertices;
		std::vector<unsigned int>& indices = meshData.indices;

		MeshData cylinderMeshData = CreateCylinder(0.05f);
		MeshData coneMeshData = CreateCone(0.12f, 0.15f);
		for (Vertex& v : coneMeshData.vertices)
		{
			v.position += UnitVector3f::Up() * 1.f;
		}
		for (unsigned int& i : coneMeshData.indices)
		{
			i += static_cast<unsigned int>(cylinderMeshData.vertices.size());
		}
		vertices.insert(end(vertices), begin(cylinderMeshData.vertices), end(cylinderMeshData.vertices));
		vertices.insert(end(vertices), begin(coneMeshData.vertices), end(coneMeshData.vertices));
		indices.insert(end(indices), begin(cylinderMeshData.indices), end(cylinderMeshData.indices));
		indices.insert(end(indices), begin(coneMeshData.indices), end(coneMeshData.indices));

		return meshData;
	}

	MeshData<Vertex> ShapeCreator3000::CreateQuad()
	{
		MeshData<Vertex> meshData;

		meshData.vertices.resize(4);

		meshData.vertices[0].position = Point3f(-0.5f, -0.5f, 0);
		meshData.vertices[1].position = Point3f(-0.5f, 0.5f, 0);
		meshData.vertices[2].position = Point3f(0.5f, 0.5f, 0);
		meshData.vertices[3].position = Point3f(0.5f, -0.5f, 0);

		meshData.indices =
		{
			0, 1, 2,
			0, 2, 3
		};

		return meshData;
	}

	MeshData<Vertex> ShapeCreator3000::CreateSkyBox(const Vector3f& size)
	{
		Vertex north_bottomLeft;
		north_bottomLeft.position = Point3f(size.x, -size.y, size.z);
		north_bottomLeft.uv = Vector2f::Zero();

		Vertex north_bottomRight;
		north_bottomRight.position = Point3f(-size.x, -size.y, size.z);
		north_bottomRight.uv = Vector2f(1, 0);

		Vertex north_topLeft;
		north_topLeft.position = Point3f(size.x, size.y, size.z);
		north_topLeft.uv = Vector2f(0, 1);

		Vertex north_topRight;
		north_topRight.position = Point3f(-size.x, size.y, size.z);
		north_topRight.uv = Vector2f::One();

		Vertex west_bottomLeft;
		west_bottomLeft.position = Point3f(-size.x, -size.y, size.z);
		west_bottomLeft.uv = Vector2f::Zero();

		Vertex west_bottomRight;
		west_bottomRight.position = Point3f(-size.x, -size.y, -size.z);
		west_bottomRight.uv = Vector2f(1, 0);

		Vertex west_topLeft;
		west_topLeft.position = Point3f(-size.x, size.y, size.z);
		west_topLeft.uv = Vector2f(0, 1);

		Vertex west_topRight;
		west_topRight.position = Point3f(-size.x, size.y, -size.z);
		west_topRight.uv = Vector2f::One();

		Vertex south_bottomLeft;
		south_bottomLeft.position = Point3f(-size.x, -size.y, -size.z);
		south_bottomLeft.uv = Vector2f::Zero();

		Vertex south_bottomRight;
		south_bottomRight.position = Point3f(size.x, -size.y, -size.z);
		south_bottomRight.uv = Vector2f(1, 0);

		Vertex south_topLeft;
		south_topLeft.position = Point3f(-size.x, size.y, -size.z);
		south_topLeft.uv = Vector2f(0, 1);

		Vertex south_topRight;
		south_topRight.position = Point3f(size.x, size.y, -size.z);
		south_topRight.uv = Vector2f::One();

		Vertex east_bottomLeft;
		east_bottomLeft.position = Point3f(size.x, -size.y, -size.z);
		east_bottomLeft.uv = Vector2f::Zero();

		Vertex east_bottomRight;
		east_bottomRight.position = Point3f(size.x, -size.y, size.z);
		east_bottomRight.uv = Vector2f(1, 0);

		Vertex east_topLeft;
		east_topLeft.position = Point3f(size.x, size.y, -size.z);
		east_topLeft.uv = Vector2f(0, 1);

		Vertex east_topRight;
		east_topRight.position = Point3f(size.x, size.y, size.z);
		east_topRight.uv = Vector2f::One();

		Vertex up_bottomLeft;
		up_bottomLeft.position = Point3f(size.x, size.y, size.z);
		up_bottomLeft.uv = Vector2f::Zero();

		Vertex up_bottomRight;
		up_bottomRight.position = Point3f(-size.x, size.y, size.z);
		up_bottomRight.uv = Vector2f(1, 0);

		Vertex up_topLeft;
		up_topLeft.position = Point3f(size.x, size.y, -size.z);
		up_topLeft.uv = Vector2f(0, 1);

		Vertex up_topRight;
		up_topRight.position = Point3f(-size.x, size.y, -size.z);
		up_topRight.uv = Vector2f::One();

		Vertex down_bottomLeft;
		down_bottomLeft.position = Point3f(-size.x, -size.y, size.z);
		down_bottomLeft.uv = Vector2f::Zero();

		Vertex down_bottomRight;
		down_bottomRight.position = Point3f(size.x, -size.y, size.z);
		down_bottomRight.uv = Vector2f(1, 0);

		Vertex down_topLeft;
		down_topLeft.position = Point3f(-size.x, -size.y, -size.z);
		down_topLeft.uv = Vector2f(0, 1);

		Vertex down_topRight;
		down_topRight.position = Point3f(size.x, -size.y, -size.z);
		down_topRight.uv = Vector2f::One();

		std::vector<Vertex> vertices =
		{
			north_bottomLeft, north_topLeft, north_topRight, north_bottomRight,
			west_bottomLeft, west_topLeft, west_topRight, west_bottomRight,
			south_bottomLeft, south_topLeft, south_topRight, south_bottomRight,
			east_bottomLeft, east_topLeft, east_topRight, east_bottomRight,
			up_bottomLeft, up_topLeft, up_topRight, up_bottomRight,
			down_bottomLeft, down_topLeft, down_topRight, down_bottomRight
		};

		std::vector<unsigned int> indices =
		{
			2, 1, 0,
			3, 2, 0,
			6, 5, 4,
			7, 6, 4,
			10, 9, 8,
			11, 10, 8,
			14, 13, 12,
			15, 14, 12,
			18, 17, 16,
			19, 18, 16,
			22, 21, 20,
			23, 22, 20
		};

		return MeshData<Vertex>(vertices, indices);
	}
}