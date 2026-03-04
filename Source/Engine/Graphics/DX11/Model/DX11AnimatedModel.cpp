#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11AnimatedModel.hpp"

#ifdef _WIN32

namespace Simple
{
	
	DX11AnimatedModel::DX11AnimatedModel(std::vector<DX11Mesh>&& meshData, const Skeleton& skeleton, const std::string& name, const std::filesystem::path& path,
		ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		: mContext(context)
		, mName(name)
		, mPath(std::filesystem::relative(path))
		, mSkeleton(skeleton)
		, mMeshes(std::move(meshData))
	{
		device;
		mBoundingBox = CreateBoundingBox(std::views::transform(mMeshes, [](const DX11Mesh& mesh) { return mesh.GetMeshData(); }));
	}

	const AABB3f& DX11AnimatedModel::GetBoundingBox() const
	{
		return mBoundingBox;
	}

	const std::string& DX11AnimatedModel::GetName() const
	{
		return mName;
	}

	const std::filesystem::path& DX11AnimatedModel::GetPath() const
	{
		return mPath;
	}

	const Skeleton& DX11AnimatedModel::GetSkeleton() const
	{
		return mSkeleton;
	}

	void DX11AnimatedModel::Render()
	{
		for (auto& mesh : mMeshes)
		{
			mesh.Render();
		}
	}
}

#endif