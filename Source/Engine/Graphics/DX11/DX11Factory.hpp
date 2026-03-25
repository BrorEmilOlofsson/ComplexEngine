#pragma once
#ifdef _WIN32

#include <filesystem>

#include <wrl/client.h>
#include <d3d11.h>

#include "Engine/Math/Vector2.hpp"


namespace CLX
{

	class DX11Factory final
	{
	public:

		[[nodiscard]] static D3D11_VIEWPORT CreateViewport(Vector2ui windowSize);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Resource> GetBackBuffer(IDXGISwapChain& swapChain);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDepthBuffer(ID3D11Device& device);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(ID3D11Device& device, Vector2ui windowSize);
		
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRasterizerState(ID3D11Device& device, D3D11_RASTERIZER_DESC desc);
		[[nodiscard]] static D3D11_RASTERIZER_DESC CreateRasterizerDesc_BackfaceCulling();
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRasterizerState_BackfaceCulling(ID3D11Device& device);

		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSamplerState(ID3D11Device& device);

		[[nodiscard]] static void CreateDeviceAndContext(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context);

		[[nodiscard]] static Microsoft::WRL::ComPtr<IDXGISwapChain> CreateSwapChain(HWND hwnd, const Vector2ui windowSize, Microsoft::WRL::ComPtr<ID3D11Device> device);
		[[nodiscard]] static Microsoft::WRL::ComPtr<IDXGISwapChain> CreateSwapChainAndDeviceAndContext(HWND hwnd, const Vector2ui windowSize, 
			Microsoft::WRL::ComPtr<ID3D11Device>& aDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context);

		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateConstantBuffer(ID3D11Device& device, const unsigned int size);
	
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTexture(ID3D11Device& device, Vector2ui size, const void* memory);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateRenderTargetTexture(ID3D11Device& device, Vector2ui size);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateRenderTargetTexture(ID3D11Device& device, D3D11_TEXTURE2D_DESC desc);

		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(ID3D11Device& device, ID3D11Resource& resource, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> desc = {});
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(ID3D11Device& device, ID3D11Resource& resource, std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> desc = {});

		[[nodiscard]] static D3D11_TEXTURE2D_DESC CreateRenderTargetTextureDesc(Vector2ui size);

		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateObjectIDStagingTexture(ID3D11Device& device, Vector2ui size);
		[[nodiscard]] static D3D11_TEXTURE2D_DESC CreateObjectSelectionTextureDesc(Vector2ui size);

		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11VertexShader> CreateVertexShaderAndInputLayout(const std::filesystem::path& path, ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11PixelShader> CreatePixelShader(const std::filesystem::path& path, ID3D11Device& device);
	
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateBuffer(ID3D11Device& device, const D3D11_BUFFER_DESC& desc, const D3D11_SUBRESOURCE_DATA* data);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device& device, const void* data, unsigned int size);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateIndexBuffer(ID3D11Device& device, const void* data, unsigned int size);
		
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateInstanceBuffer(ID3D11Device& device, unsigned int size);

		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadDDS(ID3D11Device& device, ID3D11DeviceContext& context, const std::filesystem::path& fileName);
		[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadNonDDS(ID3D11Device& device, const std::filesystem::path& fileName);
	};
}

#endif