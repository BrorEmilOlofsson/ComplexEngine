#pragma once
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Utility/MeshData.hpp"
#include <filesystem>
#include <ranges>

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>
#include "Engine/Graphics/DX11/DX11VertexBuffer.hpp"

namespace CLX
{


	[[nodiscard]] constexpr AABB3f CreateBoundingBox(const auto& meshDatas)
	{
		AABB3f boundingBox;
		for (const auto& meshData : meshDatas)
		{
            auto positionRange = meshData.vertices | std::views::transform([](const Vertex& vertex) { return vertex.position; });
			const auto meshBoundingBox = CreateAABB3FromPoints(positionRange);
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

		[[nodiscard]] const DX11VertexBuffer& GetVertexBuffer() const
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

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		MeshData<Vertex> mMeshData;

		DX11VertexBuffer mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		std::filesystem::path mName;
		std::filesystem::path mRelativePath;

		AABB3f mBoundingBox;

	};
}

#endif