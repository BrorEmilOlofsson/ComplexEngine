#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11LoadFunctions.hpp"
#include <External/TheGameAssembly/FBXImporter/Source/Importer.h>
#include "Utility/TGA/TGAAdapterFunctions.hpp"
#include "Utility/Algorithm.hpp"
#include "Graphics/Model/Skeleton.hpp"
#include <External/assimp/Importer.hpp>
#include <External/assimp/scene.h>
#include <External/assimp/postprocess.h>

namespace Simple
{

	[[nodiscard]] constexpr Point3f ToPoint3(const aiVector3f& vec)
	{
		return Point3f(vec.x, vec.y, vec.z);
	}

	[[nodiscard]] constexpr UnitVector3f ToUnitVector3(const aiVector3f& vec)
	{
		return UnitVector3f(vec.x, vec.y, vec.z);
	}

	std::vector<Vertex> ToVertices(const aiMesh& inMesh)
	{
		std::vector<Vertex> vertices(inMesh.mNumVertices);

		for (unsigned int i = 0; i < inMesh.mNumVertices; ++i)
		{
			Vertex& vertex = vertices[i];
			const aiVector3f& pos = inMesh.mVertices[i];
			vertex.position = ToPoint3(pos);

			if (inMesh.HasNormals())
			{
				vertex.normal = ToUnitVector3(inMesh.mNormals[i]);
			}

			if (inMesh.HasTangentsAndBitangents())
			{
				vertex.tangent = ToUnitVector3(inMesh.mTangents[i]);
				vertex.bitangent = ToUnitVector3(inMesh.mBitangents[i]);
			}

			if (inMesh.HasVertexColors(i))
			{
				const aiColor4D& color = inMesh.mColors[0][i];
				vertex.color = Color(color.r, color.g, color.b, color.a);
			}

			if (inMesh.HasTextureCoords(i))
			{
				vertex.uv = Vector2f(inMesh.mTextureCoords[i]->x, inMesh.mTextureCoords[i]->y);
			}

		}
		return vertices;
	}

	std::vector<unsigned int> ToIndices(const aiMesh& inMesh)
	{
		std::vector<unsigned int> indices(inMesh.mNumFaces * 3);
		for (unsigned int i = 0; i < inMesh.mNumFaces; ++i)
		{
			const aiFace& face = inMesh.mFaces[i];
			assert(face.mNumIndices == 3 && "Non-triangulated mesh found!");
			indices[i * 3 + 0] = face.mIndices[0];
			indices[i * 3 + 1] = face.mIndices[1];
			indices[i * 3 + 2] = face.mIndices[2];
		}
		return indices;
	}

	MeshData<Vertex> ToMeshData(const aiMesh& inMesh)
	{
		return MeshData<Vertex>
		{
			.vertices = ToVertices(inMesh),
			.indices = ToIndices(inMesh)
		};
	}

	std::vector<MeshData<Vertex>> ToMeshDatas(const aiScene& scene)
	{
		if (scene.mNumMeshes == 0)
		{
			return {};
		}
		unsigned int num = scene.mNumMeshes;

		std::vector<MeshData<Vertex>> meshDatas;
		meshDatas.reserve(num);


		for (unsigned int i = 0; i < num; ++i)
		{
			meshDatas.push_back(ToMeshData(*scene.mMeshes[i]));
		}

		return meshDatas;
	}

	static std::expected<MeshData<Vertex>, std::string> LoadMeshInternal(const std::filesystem::path& path)
	{
		TGA::FBX::Mesh tgaMesh;
		PROFILER_BEGIN("TGA Load Mesh");
		TGA::FBX::FbxImportStatus status = TGA::FBX::Importer::LoadMeshA(path.string(), tgaMesh);
		PROFILER_END();


		//Assimp::Importer importer;

		//const aiScene* scene = importer.ReadFile(
		//	path.string().c_str(),  // path to your .fbx
		//	aiProcess_Triangulate            // Convert everything to triangles
		//	| aiProcess_JoinIdenticalVertices // Merge duplicate vertices
		//	| aiProcess_CalcTangentSpace      // Generate tangents/bitangents
		//	| aiProcess_GenSmoothNormals      // Generate normals if missing
		//	| aiProcess_FlipUVs               // If your engine's UV origin differs
		//	| aiProcess_LimitBoneWeights      // (Optional, for animation)
		//);

		//std::vector<MeshData<Vertex>> meshDatas = ToMeshDatas(*scene);



		if (!status)
		{
			return std::unexpected("Failed to load mesh: " + path.string() + " with error code: " + std::to_string(static_cast<int>(status.Result)));
		}

		MeshData<Vertex> meshData = LoadMeshData(tgaMesh);
		return meshData;
	}

	std::expected<DX11Mesh, std::string> LoadDX11Mesh(const std::filesystem::path& path, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		TGA::FBX::Importer::InitImporter();
		PROFILER_FUNCTION(profiler::colors::Brown500);
		const std::filesystem::path absolutePath = std::filesystem::absolute(path);

		auto meshData = LoadMeshInternal(absolutePath);
		if (!meshData.has_value())
		{
			return std::unexpected(meshData.error());
		}

		const std::filesystem::path fileName = ConvertFilePathToPrettyName(path);
		return DX11Mesh(meshData.value(), fileName, path, device, context);
	}

	/*const Skeleton* LoadSkeleton(const std::string& path)
	{
		TGA::FBX::Mesh tgaMesh;
		TGA::FBX::FbxImportStatus status = TGA::FBX::Importer::LoadMeshA(std::filesystem::absolute(path).string(), tgaMesh);
		assert(status && "Failed to LoadMesh from FBXImporter");

		LoadAndCacheSkeleton(aRelativePath, tgaMesh);
		skeleton = GetSkeleton(aRelativePath.c_str());

		if (skeleton == nullptr)
		{
			assert(false && "Failed to load and cache skeleton");
			return nullptr;
		}

		return skeleton;
	}*/
}