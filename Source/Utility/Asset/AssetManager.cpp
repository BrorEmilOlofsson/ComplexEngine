#include "Utility/Precompiled/UtilityPch.hpp"
#include "AssetManager.hpp"

namespace Simple
{

	using LoadFunction = std::function<void(const std::filesystem::path&, AssetManager&, const AssetLoader&)>;

	static void LoadAssets(AssetManager& assetManager, const AssetLoader& assetLoader, const std::filesystem::path& assetsPath, const std::unordered_map<std::wstring_view, LoadFunction>& loaderMap)
	{
		PROFILER_FUNCTION(profiler::colors::Teal300);
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(assetsPath))
		{
			PROFILER_BEGIN("Load Asset");
			const auto& path = dirEntry.path();
			const auto extension = path.extension();
			auto it = loaderMap.find(extension.native());
			if (it == loaderMap.end())
			{
				continue; // Skip if no loader is registered for this extension
			}
			it->second(std::filesystem::absolute(path), assetManager, assetLoader);
			PROFILER_END();
		}
	}

	constexpr std::string_view PixelShaderPostfix = "PS";
	constexpr std::string_view VertexShaderPostfix = "VS";

	enum class eShaderT
	{
		Pixel,
		Vertex
	};

	std::optional<eShaderT> GetShaderType(const std::filesystem::path& path)
	{
		if (path.extension() != std::filesystem::path(".cso"))
		{
			return std::nullopt;
		}
		const std::string name = path.stem().string();
		const std::string postfix = name.substr(name.size() - 2, 2);
		if (postfix == PixelShaderPostfix)
		{
			return eShaderT::Pixel;
		}
		else if (postfix == VertexShaderPostfix)
		{
			return eShaderT::Vertex;
		}

		return std::nullopt;
	}

	std::unordered_map<std::wstring_view, LoadFunction> CreateLoaderMap()
	{
		std::unordered_map<std::wstring_view, LoadFunction> loaderMap;

		loaderMap[L".dds"] = [](const std::filesystem::path& path, AssetManager& assetManager, const AssetLoader& assetLoader)
			{
				PROFILER_BEGIN("Load DDS");
				TextureAsset asset = assetLoader.LoadTexture(path);
				if (asset)
				{
					assetManager.AddTexture(path, asset);
				}
				PROFILER_END();
			};

		loaderMap[L".fbx"] = [](const std::filesystem::path& path, AssetManager& assetManager, const AssetLoader& assetLoader)
			{
				PROFILER_BEGIN("Load FBX");
				MeshAsset asset = assetLoader.LoadMesh(path);
				if (asset)
				{
					assetManager.AddMesh(path, asset);
				}
				PROFILER_END();
			};

		loaderMap[L".cso"] = [](const std::filesystem::path& path, AssetManager& assetManager, const AssetLoader& assetLoader)
			{
				const std::string s = "Load CSO: " + path.string();
				PROFILER_BEGIN(s.c_str());

				auto shaderType = GetShaderType(path);

				if (!shaderType)
				{
					return;
				}

				switch (shaderType.value())
				{
				case eShaderT::Pixel:
				{
					auto asset = assetLoader.LoadPixelShader(path);
					if (asset)
					{
						assetManager.AddPixelShader(path, asset);
					}
					break;
				}
				case eShaderT::Vertex:
				{
					PROFILER_BEGIN("Load vertex shader");
					auto asset = assetLoader.LoadVertexShader(path);
					PROFILER_END();
					if (asset)
					{
						PROFILER_BEGIN("Add shader");
						assetManager.AddVertexShader(path, asset);
						PROFILER_END();
					}
					break;
				}
				default:
					break;
				}


				PROFILER_END();
			};

		return loaderMap;
	}

	void AssetManager::LoadAssets()
	{
		PROFILER_FUNCTION(profiler::colors::TealA200);
		::Simple::LoadAssets(*this, mAssetLoader, SIMPLE_DIR_ASSETS, CreateLoaderMap());
		::Simple::LoadAssets(*this, mAssetLoader, SIMPLE_DIR_SHADERS, CreateLoaderMap());

		if (mDefaultLoader)
		{
			mDefaultLoader(*this);
		}
	}
}