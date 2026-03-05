#pragma once
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"
#include "Engine/Asset/AssetTypes/MeshAsset.hpp"
#include "Engine/Asset/AssetTypes/PixelShaderAsset.hpp"
#include "Engine/Asset/AssetTypes/VertexShaderAsset.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include <array>

namespace CLX
{

	struct MeshComponent final
	{
		std::array<TextureAssetHandle, 3> textures;
		MeshAssetHandle mesh;
		PixelShaderAssetHandle pixelShader;
		VertexShaderAssetHandle vertexShader;

		MeshComponent() = default;

		MeshComponent(const MeshComponent& other)
			: textures(other.textures)
			, mesh(other.mesh)
			, pixelShader(other.pixelShader)
			, vertexShader(other.vertexShader)
		{
		}

		MeshComponent& operator=(const MeshComponent& other)
		{
			if (this != &other)
			{
				textures = other.textures;
				mesh = other.mesh;
				pixelShader = other.pixelShader;
				vertexShader = other.vertexShader;
			}
			return *this;
		}
	};

	void Initialize(MeshComponent& meshComponent, const Blackboard& blackboard);
	void RegisterMeshComponent();
}