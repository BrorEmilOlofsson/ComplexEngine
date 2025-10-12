#pragma once
#include <wrl/client.h>
#include <string>
#include <filesystem>

struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;
struct ID3D11Device;

namespace Simple
{
	class DX11Texture final
	{
	public:

		DX11Texture() = default;
		explicit DX11Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, const std::filesystem::path& relativePath);

		void Bind(const unsigned int slot);
		void Bind();
		void Unbind() const;

		void SetSlot(const unsigned int slot);

		unsigned int GetSlot() const;
		const std::string& GetName() const;
		const std::filesystem::path& GetRelativePath() const;
		ID3D11ShaderResourceView* GetShaderResourceView();

	private:

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
		std::string mName;
		std::filesystem::path mRelativePath;
		unsigned int mSlot = 0;
	};
}