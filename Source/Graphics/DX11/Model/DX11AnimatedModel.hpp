#pragma once
#include <vector>
#include <filesystem>
#include <string>
#include "Utility/Shapes/AABB3.hpp"
#include "Graphics/Model/Bone.hpp"

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>
#include "Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Graphics/Model/Skeleton.hpp"

namespace Simple
{
	class DX11AnimatedModel final
	{
	public:

		DX11AnimatedModel(std::vector<DX11Mesh>&& meshData, const Skeleton& skeleton, const std::string& modelName, const std::filesystem::path& path,
			ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);

		DX11AnimatedModel(DX11AnimatedModel&&) noexcept = default;
		DX11AnimatedModel(const DX11AnimatedModel&) = delete;
		DX11AnimatedModel& operator=(DX11AnimatedModel&&) noexcept = default;
		DX11AnimatedModel& operator=(const DX11AnimatedModel&) = delete;

		[[nodiscard]] const AABB3f& GetBoundingBox() const;
		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] const std::filesystem::path& GetPath() const;
		[[nodiscard]] const Skeleton& GetSkeleton() const;

		void Render();

	private:

		std::vector<DX11Mesh> mMeshes;

		std::string mName;
		std::filesystem::path mPath;

		AABB3f mBoundingBox;

		Skeleton mSkeleton;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	};
}

#endif