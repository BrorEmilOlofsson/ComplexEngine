#pragma once
#include "Utility/Shapes/AABB3.hpp"
#include "Utility/MeshData.hpp"
#include <string>
#include <filesystem>

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>

namespace Simple
{


	[[nodiscard]] constexpr AABB3f CreateBoundingBox(const auto& meshDatas)
	{
		AABB3f boundingBox;
		for (const auto& meshData : meshDatas)
		{
			const auto meshBoundingBox = CreateAABB3FromPoints(meshData.vertices, [](const Vertex& vertex) { return vertex.position; });
			boundingBox = MinMax(boundingBox, meshBoundingBox);
		}
		return boundingBox;
	}

	class DX11Mesh final
	{
	public:

		DX11Mesh(const MeshData<Vertex>& meshData, const std::filesystem::path& meshName, const std::filesystem::path& relativePath,
			ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);

		DX11Mesh(DX11Mesh&&) noexcept = default;
		DX11Mesh(const DX11Mesh&) = delete;
		DX11Mesh& operator=(DX11Mesh&&) noexcept = default;
		DX11Mesh& operator=(const DX11Mesh&) = delete;

		[[nodiscard]] const AABB3f& GetBoundingBox() const;
		[[nodiscard]] const std::filesystem::path& GetName() const;
		[[nodiscard]] const std::filesystem::path& GetPath() const;

		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer() const
		{
			return mVertexBuffer;
		}

		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer() const
		{
			return mIndexBuffer;
		}

		[[nodiscard]] const MeshData<Vertex>& GetMeshData() const
		{
			return mMeshData;
		}

		void Render();

	private:

		MeshData<Vertex> mMeshData;

		std::filesystem::path mName;
		std::filesystem::path mRelativePath;

		AABB3f mBoundingBox;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	};
}

#endif