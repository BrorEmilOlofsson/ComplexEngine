#pragma once

#ifdef _WIN32

#include <string>
#include <filesystem>

#include "Engine/Win/WinDefines.hpp"

#include <wrl/client.h>
#include <d3d11.h>

namespace CLX
{
	class DX11Texture final
	{
	public:

		DX11Texture() = default;
		explicit DX11Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);

		void Bind(const uint32_t slot);
		void Bind();
		void Unbind() const;

		void SetSlot(const uint32_t slot);

		uint32_t GetSlot() const;
		ID3D11ShaderResourceView* GetShaderResourceView();

	private:

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
		uint32_t mSlot = 0;
	};
}

#endif