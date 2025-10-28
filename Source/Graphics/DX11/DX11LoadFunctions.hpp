#pragma once

#ifdef _WIN32

#include "Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Graphics/DX11/Model/DX11Model.hpp"
#include <expected>
#include <string>


namespace Simple
{

	[[nodiscard]] std::expected<DX11Mesh, std::string> LoadDX11Mesh(const std::filesystem::path& path, 
		struct ID3D11Device& device, Microsoft::WRL::ComPtr<struct ID3D11DeviceContext> context);


	[[nodiscard]] std::expected<DX11Model, std::string> LoadDX11Model(const std::filesystem::path& path,
		struct ID3D11Device& device, Microsoft::WRL::ComPtr<struct ID3D11DeviceContext> context);
}

#endif