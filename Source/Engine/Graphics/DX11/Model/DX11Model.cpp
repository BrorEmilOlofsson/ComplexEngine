#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11Model.hpp"

#undef min
#undef max

namespace CLX
{

	DX11Model::DX11Model(std::vector<DX11Mesh>&& meshData, const std::string& name, const std::filesystem::path& path,
		ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		: mContext(context)
		, mName(name)
		, mPath(path)
		, mMeshes(std::move(meshData))
	{
		device;
		mBoundingBox = CreateBoundingBox(std::views::transform(mMeshes, [](const DX11Mesh& mesh) { return mesh.GetMeshData(); }));
	}

	const AABB3f& DX11Model::GetBoundingBox() const
	{
		return mBoundingBox;
	}

	const std::string& DX11Model::GetName() const
	{
		return mName;
	}

	const std::filesystem::path& DX11Model::GetPath() const
	{
		return mPath;
	}
}