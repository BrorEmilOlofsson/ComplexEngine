#pragma once
#include <functional>
#include <filesystem>
#include "Asset.hpp"
#include "Utility/Asset/AssetTypes.hpp"

namespace Simple
{

	class AssetLoader final
	{
	public:

		AssetLoader() = default;

		explicit AssetLoader(std::function<TextureAsset(const std::filesystem::path&)> textureLoader, 
			std::function<MeshAsset(const std::filesystem::path&)> meshLoader, 
			std::function<PixelShaderAsset(const std::filesystem::path&)> pixelShaderLoader,
			std::function<VertexShaderAsset(const std::filesystem::path&)> vertexShaderLoader)
			: mTextureLoader(std::move(textureLoader))
			, mMeshLoader(std::move(meshLoader))
			, mPixelShaderLoader(std::move(pixelShaderLoader))
			, mVertexShaderLoader(std::move(vertexShaderLoader))
		{
		}

		[[nodiscard]] TextureAsset LoadTexture(const std::filesystem::path& path) const
		{
			return mTextureLoader(path);
		}

		[[nodiscard]] MeshAsset LoadMesh(const std::filesystem::path& path) const
		{
			return mMeshLoader(path);
		}

		[[nodiscard]] PixelShaderAsset LoadPixelShader(const std::filesystem::path& path) const
		{
			return mPixelShaderLoader(path);
		}

		[[nodiscard]] VertexShaderAsset LoadVertexShader(const std::filesystem::path& path) const
		{
			return mVertexShaderLoader(path);
		}

	private:

		std::function<TextureAsset(const std::filesystem::path&)> mTextureLoader;
		std::function<MeshAsset(const std::filesystem::path&)> mMeshLoader;
		std::function<PixelShaderAsset(const std::filesystem::path&)> mPixelShaderLoader;
		std::function<VertexShaderAsset(const std::filesystem::path&)> mVertexShaderLoader;
	};
}