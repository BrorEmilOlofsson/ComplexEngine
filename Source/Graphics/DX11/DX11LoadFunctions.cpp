#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11LoadFunctions.hpp"
#include <External/TheGameAssembly/FBXImporter/Source/Importer.h>
#include "Utility/TGA/TGAAdapterFunctions.hpp"
#include "Utility/Algorithm.hpp"

namespace Simple
{

	std::expected<DX11Mesh, std::string> LoadDX11Mesh(const std::filesystem::path& path, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		TGA::FBX::Importer::InitImporter();
		PROFILER_FUNCTION(profiler::colors::Brown500);
		const std::filesystem::path absolutePath = std::filesystem::absolute(path);

		TGA::FBX::Mesh tgaMesh;
		PROFILER_BEGIN("TGA Load Mesh");
		TGA::FBX::FbxImportStatus status = TGA::FBX::Importer::LoadMeshA(absolutePath.string(), tgaMesh);
		PROFILER_END();

		if (!status)
		{
			return std::unexpected("Failed to load mesh: " + absolutePath.string() + " with error code: " + std::to_string(static_cast<int>(status.Result)));
		}

		MeshData<Vertex> meshData = LoadMeshData(tgaMesh);
		const std::filesystem::path fileName = ConvertFilePathToPrettyName(path);
		return DX11Mesh(meshData, fileName, path, device, context);
	}
}