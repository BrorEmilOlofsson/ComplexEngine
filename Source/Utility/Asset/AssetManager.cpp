#include "Utility/Precompiled/UtilityPch.hpp"
#include "AssetManager.hpp"
#include <queue>

namespace Simple
{

	/*Skeleton ToSkeleton(const TGA::FBX::Mesh& tgaMesh)
	{
		Skeleton skeleton;
		skeleton.mName = ConvertFilePathToPrettyName(tgaMesh.Name);

		if (tgaMesh.Skeleton.GetRoot())
		{
			skeleton.mJoints.resize(tgaMesh.Skeleton.Bones.size());
			skeleton.mJointNames.resize(skeleton.mJoints.size());

			for (size_t i = 0; i < tgaMesh.Skeleton.Bones.size(); ++i)
			{
				Matrix4x4f bindPoseInverse;

				bindPoseInverse(1, 1) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m11;
				bindPoseInverse(1, 2) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m12;
				bindPoseInverse(1, 3) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m13;
				bindPoseInverse(1, 4) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m14;

				bindPoseInverse(2, 1) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m21;
				bindPoseInverse(2, 2) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m22;
				bindPoseInverse(2, 3) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m23;
				bindPoseInverse(2, 4) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m24;

				bindPoseInverse(3, 1) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m31;
				bindPoseInverse(3, 2) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m32;
				bindPoseInverse(3, 3) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m33;
				bindPoseInverse(3, 4) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m34;

				bindPoseInverse(4, 1) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m41;
				bindPoseInverse(4, 2) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m42;
				bindPoseInverse(4, 3) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m43;
				bindPoseInverse(4, 4) = tgaMesh.Skeleton.Bones[i].BindPoseInverse.m44;

				Joint joint;

				joint.mBindPoseInverse = bindPoseInverse;
				joint.mName = tgaMesh.Skeleton.Bones[i].Name;
				joint.mParent = tgaMesh.Skeleton.Bones[i].ParentIdx;
				joint.mChildren = tgaMesh.Skeleton.Bones[i].Children;

				skeleton.mJointNameToIndex.insert({ joint.mName, i });
				skeleton.mJointNames[i] = joint.mName;
				skeleton.mJoints[i] = joint;
			}
		}
		return skeleton;
	}*/

	using LoadFunction = std::function<void(const std::filesystem::path&, AssetManager&)>;


	struct AssetLoadSorter
	{

		static std::size_t GetLoadIndex(const std::filesystem::path& path)
		{
			const std::string extension = path.extension().string();
			if (extension == ".fbx" || extension == ".cso" || extension == ".dds")
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		bool operator()(const std::filesystem::path& path1, const std::filesystem::path& path2)
		{
			return GetLoadIndex(path1) < GetLoadIndex(path2);
		}
	};

	std::priority_queue<std::filesystem::path, std::vector<std::filesystem::path>, AssetLoadSorter> GetLoadOrder(const std::filesystem::path& assetsPath, const std::unordered_map<std::wstring_view, LoadFunction>& loaderMap)
	{
		PROFILER_FUNCTION();
		
		std::priority_queue<std::filesystem::path, std::vector<std::filesystem::path>, AssetLoadSorter> paths;
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(assetsPath))
		{
			if (!loaderMap.contains(dirEntry.path().extension().native()))
			{
				continue;
			}
			paths.push(dirEntry.path());
		}
		return paths;
	}

	static void LoadAsset(const std::filesystem::path& path, const std::unordered_map<std::wstring_view, LoadFunction>& loaderMap, AssetManager& assetManager)
	{
		PROFILER_FUNCTION();
		const auto extension = path.extension();
		auto it = loaderMap.find(extension.native());
		if (it == loaderMap.end())
		{
			return; // Skip if no loader is registered for this extension
		}
		it->second(std::filesystem::absolute(path), assetManager);

	}

	void LoadAssets(AssetManager& assetManager, const std::filesystem::path& assetsPath, const std::unordered_map<std::wstring_view, LoadFunction>& loaderMap)
	{
		PROFILER_FUNCTION(profiler::colors::Teal300);
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(assetsPath))
		{
			LoadAsset(dirEntry.path(), loaderMap, assetManager);
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

		loaderMap[L".dds"] = [](const std::filesystem::path& path, AssetManager& assetManager)
			{
				PROFILER_BEGIN("Load DDS");
				TextureAsset asset = assetManager.GetAssetLoader().LoadTexture(path);
				if (asset)
				{
					assetManager.AddTexture(path, asset);
				}
				PROFILER_END();
			};

		loaderMap[L".fbx"] = [](const std::filesystem::path& path, AssetManager& assetManager)
			{
				PROFILER_BEGIN("Load FBX");
				auto fbxResult = assetManager.GetAssetLoader().LoadFBX(path);

				if (auto modelAsset = std::get_if<ModelAsset>(&fbxResult.model))
				{
					assetManager.AddModel(path, *modelAsset);
				}
				else if (auto animtedModelAsset = std::get_if<AnimatedModelAsset>(&fbxResult.model))
				{
					assetManager.AddAnimatedModel(path, *animtedModelAsset);
				}

				for (auto& animationAsset : fbxResult.animations)
				{
					assetManager.AddAnimation(path, animationAsset);
				}


				MeshAsset asset = assetManager.GetAssetLoader().LoadMesh(path);
				if (asset)
				{
					assetManager.AddMesh(path, asset);
				}
				PROFILER_END();
			};

		loaderMap[L".cso"] = [](const std::filesystem::path& path, AssetManager& assetManager)
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
					auto asset = assetManager.GetAssetLoader().LoadPixelShader(path);
					if (asset)
					{
						assetManager.AddPixelShader(path, asset);
					}
					break;
				}
				case eShaderT::Vertex:
				{
					PROFILER_BEGIN("Load vertex shader");
					auto asset = assetManager.GetAssetLoader().LoadVertexShader(path);
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

		loaderMap[L".ecomp"] = [](const std::filesystem::path& path, AssetManager& assetManager)
			{
				EntityCompositionAsset asset = assetManager.GetAssetLoader().LoadEntityComposition(path);
				assetManager.AddEntityCompositionAsset(path, std::move(asset));
			};

		return loaderMap;
	}

	void LoadAssetsNew(const std::filesystem::path& path, AssetManager& assetManager)
	{
		auto loaderMap = CreateLoaderMap();
		auto loadOrder = GetLoadOrder(path, loaderMap);

		while (!loadOrder.empty())
		{
			LoadAsset(loadOrder.top(), loaderMap, assetManager);
			loadOrder.pop();
		}
	}

	void AssetManager::LoadAssets()
	{
		PROFILER_FUNCTION(profiler::colors::TealA200);

		if (mDefaultLoader)
		{
			mDefaultLoader(*this);
		}

		LoadAssetsNew(std::filesystem::path(SIMPLE_DIR_SHADERS), *this);
		LoadAssetsNew(std::filesystem::path(SIMPLE_DIR_ASSETS), *this);
		//::Simple::LoadAssets(*this, std::filesystem::path(SIMPLE_DIR_ASSETS), CreateLoaderMap());
		//::Simple::LoadAssets(*this, std::filesystem::path(SIMPLE_DIR_SHADERS), CreateLoaderMap());

		
	}
}