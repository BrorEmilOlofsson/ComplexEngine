#pragma once
#include "Engine/Asset/TextureAsset.hpp"
#include "Engine/Asset/MeshAsset.hpp"
#include "Engine/Asset/PixelShaderAsset.hpp"
#include "Engine/Asset/VertexShaderAsset.hpp"
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