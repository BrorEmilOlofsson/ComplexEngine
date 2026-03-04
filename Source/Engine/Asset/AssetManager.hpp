#pragma once
#include <vector>
#include <filesystem>
#include <functional>
#include <unordered_map>
#include "Engine/Asset/AssetTypes.hpp"
#include "Engine/Asset/AssetLoader.hpp"

namespace CLX
{

	[[nodiscard]] decltype(auto) MapFind(auto& map, const auto& key)
	{
		auto it = map.find(key);
		if (it != map.end())
		{
			return it->second;
		}
		return decltype(it->second)();
	}



	class AssetManager final
	{
		template<typename HandleType>
		[[nodiscard]] static constexpr decltype(auto) ValidatedGet(auto& path, auto& map)
		{
			if (auto asset = MapFind(map, std::filesystem::absolute(path)))
			{
				return HandleType(asset);
			}
			return HandleType();
		}
	public:

		AssetManager() = default;

		void SetDefaultLoader(std::function<void(AssetManager&)> defaultLoader)
		{
			mDefaultLoader = std::move(defaultLoader);
		}

		[[nodiscard]] AssetLoader& GetAssetLoader()
		{
			return mAssetLoader;
		}

		[[nodiscard]] const AssetLoader& GetAssetLoader() const
		{
			return mAssetLoader;
		}

		void LoadAssets();

		[[nodiscard]] TextureAssetHandle GetTexture(const std::filesystem::path& path)
		{
			return ValidatedGet<TextureAssetHandle>(path, mTextureAssets);
		}

		[[nodiscard]] MeshAssetHandle GetMesh(const std::filesystem::path& path)
		{
			return ValidatedGet<MeshAssetHandle>(path, mMeshAssets);
		}

		[[nodiscard]] ModelAssetHandle GetModel(const std::filesystem::path& path)
		{
			return ValidatedGet<ModelAssetHandle>(path, mModelAssets);
		}

		[[nodiscard]] AnimatedModelAssetHandle GetAnimatedModel(const std::filesystem::path& path)
		{
			return ValidatedGet<AnimatedModelAssetHandle>(path, mAnimatedModelAssets);
		}

		[[nodiscard]] AnimationAssetHandle GetAnimation(const std::filesystem::path& path)
		{
			return ValidatedGet<AnimationAssetHandle>(path, mAnimationAssets);
		}

		[[nodiscard]] PixelShaderAssetHandle GetPixelShader(const std::filesystem::path& path)
		{
			return ValidatedGet<PixelShaderAssetHandle>(path, mPixelShaderAssets);
		}

		[[nodiscard]] VertexShaderAssetHandle GetVertexShader(const std::filesystem::path& path)
		{
			return ValidatedGet<VertexShaderAssetHandle>(path, mVertexShaderAssets);
		}

		[[nodiscard]] EntityCompositionAssetHandle GetEntityComposition(const std::filesystem::path& path)
		{
			return ValidatedGet<EntityCompositionAssetHandle>(path, mEntityCompositionAssets);
		}

		void AddTexture(const std::filesystem::path& path, TextureAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mTextureAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddModel(const std::filesystem::path& path, ModelAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mModelAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddAnimatedModel(const std::filesystem::path& path, AnimatedModelAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mAnimatedModelAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddAnimation(const std::filesystem::path& path, AnimationAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mAnimationAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddMesh(const std::filesystem::path& path, MeshAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mMeshAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddPixelShader(const std::filesystem::path& path, PixelShaderAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mPixelShaderAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		void AddVertexShader(const std::filesystem::path& path, VertexShaderAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mVertexShaderAssets[std::filesystem::absolute(path)] = std::move(asset);
		}

		EntityCompositionAssetHandle AddEntityCompositionAsset(const std::filesystem::path& path, EntityCompositionAsset asset)
		{
			if (!asset)
			{
				throw std::invalid_argument("Asset is invalid");
			}
			mEntityCompositionAssets[std::filesystem::absolute(path)] = std::move(asset);
			return GetEntityComposition(path);
		}

		void Remove(const std::filesystem::path& path)
		{
			const auto extension = path.extension();

			auto remove = [](auto& map, const std::filesystem::path& path)
				{
					map.erase(path);
				};

			if (extension == ".dds")
			{
				remove(mTextureAssets, path);
			}
			else if (extension == ".fbx")
			{
				remove(mMeshAssets, path);
				remove(mModelAssets, path);
			}
			else if (extension == ".cso")
			{
				remove(mPixelShaderAssets, path);
				remove(mVertexShaderAssets, path);
			}
			else if (extension == ".ecomp")
			{
				remove(mEntityCompositionAssets, path);
			}
		}

	private:

		std::unordered_map<std::filesystem::path, TextureAsset> mTextureAssets;
		std::unordered_map<std::filesystem::path, MeshAsset> mMeshAssets;
		std::unordered_map<std::filesystem::path, ModelAsset> mModelAssets;
		std::unordered_map<std::filesystem::path, AnimatedModelAsset> mAnimatedModelAssets;
		std::unordered_map<std::filesystem::path, AnimationAsset> mAnimationAssets;
		std::unordered_map<std::filesystem::path, PixelShaderAsset> mPixelShaderAssets;
		std::unordered_map<std::filesystem::path, VertexShaderAsset> mVertexShaderAssets;
		std::unordered_map<std::filesystem::path, EntityCompositionAsset> mEntityCompositionAssets;

		std::function<void(AssetManager&)> mDefaultLoader;

		AssetLoader mAssetLoader;
	};
}