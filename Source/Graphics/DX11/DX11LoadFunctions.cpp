#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11LoadFunctions.hpp"
#include <External/TheGameAssembly/FBXImporter/Source/Importer.h>
#include "Utility/TGA/TGAAdapterFunctions.hpp"
#include "Utility/Algorithm.hpp"
#include "Graphics/Model/Skeleton.hpp"
#include "Graphics/DX11/Model/DX11Model.hpp"
#include "Graphics/Animation/Animation.hpp"
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

	[[nodiscard]] constexpr Vector3f ToVector3(const aiVector3f& vec)
	{
		return Vector3f(vec.x, vec.y, vec.z);
	}

	[[nodiscard]] constexpr Quaternionf ToQuaternion(const aiQuaternion& quat)
	{
		return Quaternionf(quat.w, quat.x, quat.y, quat.z);
	}

	[[nodiscard]] constexpr Color ToColor(const aiColor4D& color)
	{
		return Color(color.r, color.g, color.b, color.a);
	}

	[[nodiscard]] constexpr Matrix4x4f ToMatrix(const aiMatrix4x4& mat)
	{
		return Matrix4x4f::GetTransposed(Matrix4x4f({
			mat.a1, mat.a2, mat.a3, mat.a4,
			mat.b1, mat.b2, mat.b3, mat.b4,
			mat.c1, mat.c2, mat.c3, mat.c4,
			mat.d1, mat.d2, mat.d3, mat.d4
			}));
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

			if (inMesh.HasVertexColors(0))
			{
				vertex.color = ToColor(inMesh.mColors[0][i]);
			}

			if (inMesh.HasTextureCoords(0))
			{
				vertex.uv = Vector2f(inMesh.mTextureCoords[0][i].x, 1 - inMesh.mTextureCoords[0][i].y);
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

	void HandleBones(const aiMesh& mesh, MeshData<Vertex>& meshData, std::vector<Bone>& bones, std::unordered_map<std::string, uint32_t>& boneIndexMap)
	{
		for (unsigned int b = 0; b < mesh.mNumBones; b++)
		{
			const aiBone& aiBone = *mesh.mBones[b];
			uint32_t boneIndex = 0;
			std::string boneName = aiBone.mName.C_Str();
			if (!boneIndexMap.contains(boneName))
			{
				boneIndex = static_cast<uint32_t>(bones.size());
				boneIndexMap[boneName] = boneIndex;

				Bone& bone = bones.emplace_back();
				bone.name = boneName;
				bone.inverseBindPose = Matrix4x4f::GetFastInverse(ToMatrix(aiBone.mOffsetMatrix));
				bone.parentIndex = std::numeric_limits<uint32_t>::max();
			}
			else
			{
				boneIndex = boneIndexMap[boneName];
			}

			auto addWeight = [&](Vertex& vertex, const uint32_t boneIndex, const float weight)
				{
					auto it = std::ranges::find_if(vertex.weights,
						[](float w) { return w == 0.f; });

					if (it == vertex.weights.end())
					{
						// No empty weight slot found
						return;
					}

					auto index = std::distance(vertex.weights.begin(), it);

					vertex.bones[index] = boneIndex;
					vertex.weights[index] = weight;
				};

			for (unsigned int w = 0; w < aiBone.mNumWeights; w++)
			{
				auto& weight = aiBone.mWeights[w];
				addWeight(meshData.vertices[weight.mVertexId], boneIndex, weight.mWeight);
			}
		}
	}

	void BuildSkeletonHierarchy(const aiNode* node,
		int parentIdx,
		std::vector<Bone>& bones,
		const std::unordered_map<std::string, uint32_t>& boneIndexMap)
	{
		const std::string name = node->mName.C_Str();

		// If this node is one of our bones, set its parent
		auto it = boneIndexMap.find(name);
		int thisIdx = parentIdx;
		if (it != boneIndexMap.end())
		{
			thisIdx = static_cast<int>(it->second);
			if (parentIdx >= 0) bones[thisIdx].parentIndex = static_cast<uint32_t>(parentIdx);
			else                bones[thisIdx].parentIndex = std::numeric_limits<uint32_t>::max();
		}

		for (unsigned i = 0; i < node->mNumChildren; ++i)
			BuildSkeletonHierarchy(node->mChildren[i], thisIdx, bones, boneIndexMap);
	}

	void TraverseNodes(std::vector<DX11Mesh>& meshes, std::vector<Bone>& bones, std::unordered_map<std::string, uint32_t>& boneIndexMap, const aiNode* node, const aiScene* scene, const std::filesystem::path& path,
		ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Process mesh data
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			const aiMesh& mesh = *scene->mMeshes[node->mMeshes[i]];

			MeshData meshData = ToMeshData(mesh);
			HandleBones(mesh, meshData, bones, boneIndexMap);
			const std::string meshName = mesh.mName.C_Str();

			for (Vertex& vertex : meshData.vertices)
			{
				float sum = 0.f;
				for (float weight : vertex.weights)
				{
					sum += weight;
				}

				if (sum != 0.f && sum != 1.f)
				{
					Normalize(vertex.weights[0], vertex.weights[1], vertex.weights[2], vertex.weights[3]);
				}
			}

			meshes.push_back(DX11Mesh(meshData, meshName, path, device, context));

		}

		// Process child nodes recursively
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			TraverseNodes(meshes, bones, boneIndexMap, node->mChildren[i], scene, path, device, context);
		}
	}

	Animation LoadAnimationTest(const aiAnimation& aiAnim, const std::unordered_map<std::string, uint32_t>& boneIndexMap, uint32_t skeletonBoneCount)
	{
		Animation animation;
		animation.name = aiAnim.mName.C_Str();
		animation.duration = static_cast<float>(aiAnim.mDuration);
		animation.framesPerSecond = static_cast<float>(aiAnim.mTicksPerSecond != 0.0 ? aiAnim.mTicksPerSecond : 25.0);

		
		animation.boneKeyFrames.resize(skeletonBoneCount);

		for (unsigned int channelIndex = 0; channelIndex < aiAnim.mNumChannels; ++channelIndex)
		{

			const aiNodeAnim& aiChannel = *aiAnim.mChannels[channelIndex];
			std::string channelName = aiChannel.mNodeName.C_Str();

			// Does this animated channel exist in our skeleton?
			auto it = boneIndexMap.find(channelName);
			if (it == boneIndexMap.end())
			{
				continue; // <-- skip channels not part of skeleton
			}

			const uint32_t realBoneIndex = it->second;
			BoneKeyFrames& keyframes = animation.boneKeyFrames[realBoneIndex];

			std::println("Positions numbers: {}", aiChannel.mNumPositionKeys);
			std::println("Rotation  numbers: {}", aiChannel.mNumRotationKeys);
			std::println("Scale		 numbers: {}", aiChannel.mNumScalingKeys);

			for (uint32_t i = 0; i < aiChannel.mNumPositionKeys; i++)
			{
				float t = static_cast<float>(aiChannel.mPositionKeys[i].mTime / aiAnim.mTicksPerSecond);
				
				keyframes.positionTimestamps.push_back(t);
				keyframes.positions.push_back(ToPoint3(aiChannel.mPositionKeys[i].mValue));
			}

			for (uint32_t i = 0; i < aiChannel.mNumRotationKeys; i++)
			{
				const float t = static_cast<float>(aiChannel.mRotationKeys[i].mTime / aiAnim.mTicksPerSecond);

				keyframes.rotationTimestamps.push_back(t);
				keyframes.rotations.push_back(ToQuaternion(aiChannel.mRotationKeys[i].mValue));
			}

			for (size_t i = 0; i < aiChannel.mNumScalingKeys; i++)
			{
				const float t = static_cast<float>(aiChannel.mScalingKeys[i].mTime / aiAnim.mTicksPerSecond);

				keyframes.scaleTimestamps.push_back(t);
				keyframes.scales.push_back(ToVector3(aiChannel.mScalingKeys[i].mValue));
			}
		}

		for (uint32_t i = 0; i < animation.boneKeyFrames.size(); ++i)
		{
			std::println("Bone {} keys: P={}, R={}, S={}",
				i,
				animation.boneKeyFrames[i].positions.size(),
				animation.boneKeyFrames[i].rotations.size(),
				animation.boneKeyFrames[i].scales.size()
			);
		}
		
		return animation;
	}

	std::vector<Animation> LoadAnimations(const aiScene& scene, const std::unordered_map<std::string, uint32_t>& boneIndexMap, uint32_t skeletonBoneCount)
	{
		std::vector<Animation> animations;

		if (!scene.HasAnimations())
		{
			return animations;
		}

		for (unsigned int i = 0; i < scene.mNumAnimations; ++i)
		{
			animations.push_back(LoadAnimationTest(*scene.mAnimations[i], boneIndexMap, skeletonBoneCount));
		}

		return animations;
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

		if (!status)
		{
			return std::unexpected("Failed to load mesh: " + path.string() + " with error code: " + std::to_string(static_cast<int>(status.Result)));
		}

		MeshData<Vertex> meshData = LoadMeshData(tgaMesh);
		return meshData;
	}

	std::expected<FBXResult, std::string> LoadDX11FBX(const std::filesystem::path& path, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		PROFILER_FUNCTION();
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(
			path.string().c_str(),  // path to your .fbx
			aiProcess_Triangulate            // Convert everything to triangles
			//| aiProcess_JoinIdenticalVertices // Merge duplicate vertices
			//| aiProcess_CalcTangentSpace      // Generate tangents/bitangents
			//| aiProcess_GenSmoothNormals      // Generate normals if missing
			//| aiProcess_FlipUVs               // If your engine's UV origin differs
			//| aiProcess_LimitBoneWeights      // (Optional, for animation)
		);

		if (scene == nullptr)
		{
			return std::unexpected("Failed to load model: " + path.string() + " with error: " + importer.GetErrorString());
		}


		FBXResult result;


		std::vector<DX11Mesh> meshes;

		std::vector<Bone> boneVector;
		std::unordered_map<std::string, uint32_t> boneIndexMap;


		TraverseNodes(meshes, boneVector, boneIndexMap, scene->mRootNode, scene, path, device, context);
		BuildSkeletonHierarchy(scene->mRootNode, -1, boneVector, boneIndexMap);

		std::array<Bone, GlobalMaxBones> bones;

		if (boneVector.size() >= GlobalMaxBones)
		{
			assert(false);
		}
		std::ranges::copy(boneVector, bones.begin());
		bool hasBones = !bones.empty();

		if (scene->HasAnimations())
		{
			std::println("Loading animations: {}", path.string());
			result.animations = LoadAnimations(*scene, boneIndexMap, static_cast<uint32_t>(boneVector.size()));
		}

		if (hasBones)
		{
			std::println("Loading skeleton: {}", path.string());
			result.model = DX11AnimatedModel(std::move(meshes), Skeleton(bones), std::string(scene->mName.C_Str()), path, device, context);
		}
		else
		{
			std::println("Loading non-skeleton: {}", path.string());
			result.model = DX11Model(std::move(meshes), std::string(scene->mName.C_Str()), path, device, context);
		}

		return result;
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
}