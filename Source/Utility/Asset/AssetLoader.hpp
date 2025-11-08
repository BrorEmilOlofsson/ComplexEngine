#pragma once
#include <functional>
#include <filesystem>
#include <variant>
#include "Asset.hpp"
#include "Utility/Asset/AssetTypes.hpp"

namespace Simple
{

	class AssetLoader final
	{
	public:

		AssetLoader() = default;

		[[nodiscard]] TextureAsset LoadTexture(const std::filesystem::path& path) const
		{
			return mTextureLoader(path);
		}

		[[nodiscard]] MeshAsset LoadMesh(const std::filesystem::path& path) const
		{
			return mMeshLoader(path);
		}

		[[nodiscard]] std::variant<std::monostate, ModelAsset, AnimatedModelAsset> LoadModel(const std::filesystem::path& path) const
		{
			return mModelLoader(path);
		}

		[[nodiscard]] PixelShaderAsset LoadPixelShader(const std::filesystem::path& path) const
		{
			return mPixelShaderLoader(path);
		}

		[[nodiscard]] VertexShaderAsset LoadVertexShader(const std::filesystem::path& path) const
		{
			return mVertexShaderLoader(path);
		}

		[[nodiscard]] EntityCompositionAsset LoadEntityComposition(const std::filesystem::path& path) const
		{
			return mEntityCompositionLoader(path);
		}

		void SetTextureLoader(std::function<TextureAsset(const std::filesystem::path&)> loader)
		{
			mTextureLoader = std::move(loader);
		}

		void SetMeshLoader(std::function<MeshAsset(const std::filesystem::path&)> loader)
		{
			mMeshLoader = std::move(loader);
		}

		void SetModelLoader(std::function<std::variant<std::monostate, ModelAsset, AnimatedModelAsset>(const std::filesystem::path&)> loader)
		{
			mModelLoader = std::move(loader);
		}

		void SetPixelShaderLoader(std::function<PixelShaderAsset(const std::filesystem::path&)> loader)
		{
			mPixelShaderLoader = std::move(loader);
		}

		void SetVertexShaderLoader(std::function<VertexShaderAsset(const std::filesystem::path&)> loader)
		{
			mVertexShaderLoader = std::move(loader);
		}

		void SetEntityCompositionLoader(std::function<EntityCompositionAsset(const std::filesystem::path&)> loader)
		{
			mEntityCompositionLoader = std::move(loader);
		}

	private:

		std::function<TextureAsset(const std::filesystem::path&)> mTextureLoader;
		std::function<MeshAsset(const std::filesystem::path&)> mMeshLoader;
		std::function<std::variant<std::monostate, ModelAsset, AnimatedModelAsset>(const std::filesystem::path&)> mModelLoader;
		std::function<PixelShaderAsset(const std::filesystem::path&)> mPixelShaderLoader;
		std::function<VertexShaderAsset(const std::filesystem::path&)> mVertexShaderLoader;
		std::function<EntityCompositionAsset(const std::filesystem::path&)> mEntityCompositionLoader;
	};
}