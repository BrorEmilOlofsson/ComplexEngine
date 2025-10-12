#include "Engine/Precompiled/EnginePch.hpp"
#include "NavmeshLoader.hpp"
#include "Utility/File/FileLoader.hpp"

namespace Simple
{

	static NavmeshData ConvertMeshToNavmesh(const MeshData<Vertex>& meshData)
	{
		PROFILER_FUNCTION(profiler::colors::RichRed);
		NavmeshData navmeshData;
		navmeshData.mVertices.resize(meshData.vertices.size());
		navmeshData.m3DVertices.resize(meshData.vertices.size());
		std::ranges::transform(meshData.vertices, navmeshData.mVertices.begin(), [](const Vertex& v) { return ToPoint2XZ(v.position); });
		std::ranges::transform(meshData.vertices, navmeshData.m3DVertices.begin(), [](const Vertex& v) { return v.position; });
		navmeshData.mIndices = meshData.indices;

		return navmeshData;
	}

	NavmeshData NavmeshLoader::LoadMesh(const std::filesystem::path& objFilePath)
	{
		return ConvertMeshToNavmesh(FileLoader::LoadObj(objFilePath));
	}
}
