#pragma once
#include "Utility/Asset/TextureAsset.hpp"
#include "Utility/Asset/MeshAsset.hpp"
#include "Utility/Asset/PixelShaderAsset.hpp"
#include "Utility/Asset/VertexShaderAsset.hpp"
#include <array>

namespace Simple
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