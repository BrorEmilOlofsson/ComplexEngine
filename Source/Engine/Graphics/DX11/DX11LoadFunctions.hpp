#pragma once

#ifdef _WIN32

#include "Engine/Graphics/DX11/Mesh/DX11Mesh.hpp"
#include "Engine/Graphics/DX11/Model/DX11Model.hpp"
#include "Engine/Graphics/DX11/Model/DX11AnimatedModel.hpp"
#include "Engine/Graphics/Animation/Animation.hpp"
#include <expected>
#include <string>
#include <variant>

namespace Simple
{

	[[nodiscard]] std::expected<DX11Mesh, std::string> LoadDX11Mesh(const std::filesystem::path& path, 
		struct ID3D11Device& device, Microsoft::WRL::ComPtr<struct ID3D11DeviceContext> context);

	struct FBXResult
	{
		std::variant<std::monostate, DX11Model, DX11AnimatedModel> model;
		std::vector<Animation> animations;
	};

	[[nodiscard]] std::expected<FBXResult, std::string> LoadDX11FBX(const std::filesystem::path& path,
		struct ID3D11Device& device, Microsoft::WRL::ComPtr<struct ID3D11DeviceContext> context);
}

#endif