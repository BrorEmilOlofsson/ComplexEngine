#pragma once
#include "Graphics/DX11/Mesh/DX11Mesh.hpp"
#include <expected>
#include <string>

namespace Simple
{

	[[nodiscard]] std::expected<DX11Mesh, std::string> LoadDX11Mesh(const std::filesystem::path& path, 
		struct ID3D11Device& device, Microsoft::WRL::ComPtr<struct ID3D11DeviceContext> context);
}