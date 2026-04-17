#pragma once

#ifdef _WIN32

#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Utility/MeshData.hpp"
#include <string>
#include <filesystem>

#include <d3d11.h>
#include <wrl/client.h>
#include "Engine/Graphics/DX11/Mesh/DX11Mesh.hpp"

namespace CLX
{

	class DX11Model final
	{
	public:

		DX11Model(std::vector<DX11Mesh>&& meshData, const std::string& modelName, const std::filesystem::path& path,
			ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);

		DX11Model(DX11Model&&) noexcept = default;
		DX11Model(const DX11Model&) = delete;
		DX11Model& operator=(DX11Model&&) noexcept = default;
		DX11Model& operator=(const DX11Model&) = delete;

		[[nodiscard]] const AABB3f& GetBoundingBox() const;
		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] const std::filesystem::path& GetPath() const;
		[[nodiscard]] std::span<const DX11Mesh> GetMeshes() const
		{
			return mMeshes;
        }

		[[nodiscard]] std::span<DX11Mesh> GetMeshes()
		{
			return mMeshes;
		}
	private:

		std::vector<DX11Mesh> mMeshes;

		std::string mName;
		std::filesystem::path mPath;

		AABB3f mBoundingBox;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	};
}

#endif