#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11Factory.hpp"
#include "Engine/Utility/Win/WinException.hpp"
#include <fstream>
#include <DirectXTK/DDSTextureLoader.h>
#include <stb/stb_image.h>

#ifdef _WIN32

namespace Simple
{

	D3D11_VIEWPORT DX11Factory::CreateViewport(Vector2ui windowSize)
	{
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(windowSize.x);
		viewport.Height = static_cast<float>(windowSize.y);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		return viewport;
	}

	Microsoft::WRL::ComPtr<ID3D11Resource> DX11Factory::GetBackBuffer(IDXGISwapChain& swapChain)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer = nullptr;
		const HRESULT hr = swapChain.GetBuffer(0u, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		WIN_CHECK_HRESULT(hr);

		return backBuffer;
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DX11Factory::CreateDepthBuffer(ID3D11Device& device)
	{
		HRESULT hr{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		hr = device.CreateDepthStencilState(&dsDesc, &depthStencilState);
		WIN_CHECK_HRESULT(hr);

		return depthStencilState;
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DX11Factory::CreateDepthStencilView(ID3D11Device& device, Vector2ui windowSize)
	{
		D3D11_TEXTURE2D_DESC descDepth{};
		descDepth.Width = windowSize.x;
		descDepth.Height = windowSize.y;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
		{
			const HRESULT hr = device.CreateTexture2D(&descDepth, nullptr, &depthStencilTexture);
			WIN_CHECK_HRESULT(hr);
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		descDSV.Flags = 0u;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		{
			const HRESULT hr = device.CreateDepthStencilView(depthStencilTexture.Get(), &descDSV, &depthStencilView);
			WIN_CHECK_HRESULT(hr);
		}
		return depthStencilView;
	}

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> DX11Factory::CreateRasterizerState(ID3D11Device& device, D3D11_RASTERIZER_DESC desc)
	{
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		HRESULT hr = device.CreateRasterizerState(&desc, &rasterizerState);
		WIN_CHECK_HRESULT(hr);
		return rasterizerState;
	}

	D3D11_RASTERIZER_DESC DX11Factory::CreateRasterizerDesc_BackfaceCulling()
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.MultisampleEnable = true;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		return rasterizerDesc;
	}

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> DX11Factory::CreateRasterizerState_BackfaceCulling(ID3D11Device& device)
	{
		return CreateRasterizerState(device, CreateRasterizerDesc_BackfaceCulling());
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> DX11Factory::CreateSamplerState(ID3D11Device& device)
	{
		D3D11_SAMPLER_DESC desc{};

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
		const HRESULT hr = device.CreateSamplerState(&desc, &samplerState);

		WIN_CHECK_HRESULT(hr);

		return samplerState;
	}

	void DX11Factory::CreateDeviceAndContext(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context)
	{
		UINT creationFlags = 0;
#if defined (REPORT_DX_WARNINGS)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		const HRESULT hr = D3D11CreateDevice(
			nullptr, // Use default adapter
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, // No software rasterizer
			creationFlags, // No flags
			nullptr, // Default feature levels
			0, // Number of feature levels
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			&context
		);
		
		WIN_CHECK_HRESULT(hr);
	}

	static DXGI_SWAP_CHAIN_DESC CreateSwapChainDesc(HWND hwnd, const Vector2ui windowSize)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = windowSize.x;
		swapChainDesc.BufferDesc.Height = windowSize.y;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;
		return swapChainDesc;
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain> DX11Factory::CreateSwapChain(HWND hwnd, const Vector2ui windowSize, Microsoft::WRL::ComPtr<ID3D11Device> device)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = CreateSwapChainDesc(hwnd, windowSize);

		// Get DXGI device
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		device.As(&dxgiDevice);

		// Get adapter
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		dxgiDevice->GetAdapter(&adapter);

		// Get factory
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		adapter->GetParent(__uuidof(IDXGIFactory), &factory);

		// Create swap chain
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		const HRESULT hr = factory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain);

		WIN_CHECK_HRESULT(hr);

		return swapChain;
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain> DX11Factory::CreateSwapChainAndDeviceAndContext(HWND hwnd, const Vector2ui windowSize,
		Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = CreateSwapChainDesc(hwnd, windowSize);

		UINT creationFlags = 0;
#if defined (REPORT_DX_WARNINGS)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

		const HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain,
			&device,
			nullptr,
			&context
		);

		WIN_CHECK_HRESULT(hr);

		return swapChain;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> DX11Factory::CreateConstantBuffer(ID3D11Device& device, const unsigned int byteSize)
	{
		D3D11_BUFFER_DESC bufferDescription = {};
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.ByteWidth = static_cast<UINT>(byteSize);

		return CreateBuffer(device, bufferDescription, nullptr);
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX11Factory::CreateTexture(ID3D11Device& device, Vector2ui size, const void* memory)
	{

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = size.x;
		desc.Height = size.y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceDesc = {};

		subresourceDesc.pSysMem = memory;
		subresourceDesc.SysMemPitch = size.x * 4;
		subresourceDesc.SysMemSlicePitch = size.x * size.y * 4;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
		const HRESULT hr = device.CreateTexture2D(&desc, &subresourceDesc, &texture);

		WIN_CHECK_HRESULT(hr);

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

		HRESULT result = device.CreateShaderResourceView(texture.Get(), nullptr, &shaderResourceView);

		WIN_CHECK_HRESULT(result);

		return shaderResourceView;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> DX11Factory::CreateRenderTargetTexture(ID3D11Device& device, Vector2ui size)
	{
		return CreateRenderTargetTexture(device, CreateRenderTargetTextureDesc(size));
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> DX11Factory::CreateRenderTargetTexture(ID3D11Device& device, D3D11_TEXTURE2D_DESC desc)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
		HRESULT result = device.CreateTexture2D(&desc, nullptr, &texture);
		WIN_CHECK_HRESULT(result);
		return texture;
	}

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> DX11Factory::CreateRenderTargetView(ID3D11Device& device, ID3D11Resource& resource, std::optional<D3D11_RENDER_TARGET_VIEW_DESC> desc)
	{
		const auto* descPtr = desc.has_value() ? &desc.value() : nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		HRESULT result = device.CreateRenderTargetView(&resource, descPtr, &renderTargetView);
		WIN_CHECK_HRESULT(result);
		return renderTargetView;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX11Factory::CreateShaderResourceView(ID3D11Device& device, ID3D11Resource& resource, std::optional<D3D11_SHADER_RESOURCE_VIEW_DESC> desc)
	{
		const auto* descPtr = desc.has_value() ? &desc.value() : nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		HRESULT result = device.CreateShaderResourceView(&resource, descPtr, &shaderResourceView);
		WIN_CHECK_HRESULT(result);
		return shaderResourceView;
	}

	D3D11_TEXTURE2D_DESC DX11Factory::CreateRenderTargetTextureDesc(Vector2ui size)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = size.x;
		textureDesc.Height = size.y;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		return textureDesc;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> DX11Factory::CreateObjectIDStagingTexture(ID3D11Device& device, Vector2ui size)
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = size.x;
		desc.Height = size.y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_UINT;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_STAGING;      // CPU readable
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
		device.CreateTexture2D(&desc, nullptr, &stagingTexture);
		return stagingTexture;
	}

	D3D11_TEXTURE2D_DESC DX11Factory::CreateObjectSelectionTextureDesc(Vector2ui size)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = size.x;
		textureDesc.Height = size.y;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_UINT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;

		return textureDesc;
	}

	static std::string LoadCSOData(const std::filesystem::path& path)
	{
		PROFILER_FUNCTION(profiler::colors::DeepPurple100);
		std::string data;
		const std::filesystem::path psFilepath = std::filesystem::absolute(path);

		std::ifstream psFile(psFilepath, std::ios::binary);
		data = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

		return data;
	}

	Microsoft::WRL::ComPtr<ID3D11VertexShader> DX11Factory::CreateVertexShaderAndInputLayout(const std::filesystem::path& path,
		ID3D11Device& device, Microsoft::WRL::ComPtr<ID3D11InputLayout>& inputLayout)
	{
		PROFILER_FUNCTION(profiler::colors::DeepPurple100);
		const std::string data = LoadCSOData(path);

		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		{
			PROFILER_BEGIN("DX11 create vertex shader")
			const HRESULT result = device.CreateVertexShader(data.data(), data.size(), nullptr, &vertexShader);

			WIN_CHECK_HRESULT(result);
			PROFILER_END();
		}

		{
			constexpr std::array<D3D11_INPUT_ELEMENT_DESC, 8> inputLayoutDesc
			{
				D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				D3D11_INPUT_ELEMENT_DESC{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				D3D11_INPUT_ELEMENT_DESC{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
				D3D11_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
				D3D11_INPUT_ELEMENT_DESC{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
				D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
				
			};

			PROFILER_BEGIN("DX11 create input layout")
			const HRESULT result = device.CreateInputLayout(inputLayoutDesc.data(), static_cast<UINT>(inputLayoutDesc.size()), data.data(), data.size(), &inputLayout);

			WIN_CHECK_HRESULT(result);
			PROFILER_END();

		}

		return vertexShader;
	}

	Microsoft::WRL::ComPtr<ID3D11PixelShader> DX11Factory::CreatePixelShader(const std::filesystem::path& path, ID3D11Device& device)
	{
		const std::string data = LoadCSOData(path);

		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
		const HRESULT result = device.CreatePixelShader(data.data(), data.size(), nullptr, &pixelShader);

		WIN_CHECK_HRESULT(result);

		return pixelShader;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> DX11Factory::CreateBuffer(ID3D11Device& device, const D3D11_BUFFER_DESC& desc, const D3D11_SUBRESOURCE_DATA* aData)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		const HRESULT result = device.CreateBuffer(&desc, aData, &vertexBuffer);
		WIN_CHECK_HRESULT(result);

		return vertexBuffer;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> DX11Factory::CreateVertexBuffer(ID3D11Device& device, const void* data, const unsigned int size)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = data;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		const HRESULT result = device.CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
		WIN_CHECK_HRESULT(result);

		return vertexBuffer;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> DX11Factory::CreateIndexBuffer(ID3D11Device& device, const void* data, const unsigned int size)
	{
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.ByteWidth = size;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = data;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		return CreateBuffer(device, indexBufferDesc, &indexData);
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> DX11Factory::CreateInstanceBuffer(ID3D11Device& device, const unsigned int size)
	{
		D3D11_BUFFER_DESC instanceBufferDesc = {};
		instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		instanceBufferDesc.ByteWidth = size;
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		instanceBufferDesc.MiscFlags = 0;
		instanceBufferDesc.StructureByteStride = 0;

		return CreateBuffer(device, instanceBufferDesc, nullptr);
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX11Factory::LoadDDS(ID3D11Device& device, ID3D11DeviceContext& context, const std::filesystem::path& fileName)
	{
		const std::filesystem::path texturePath = std::filesystem::absolute("") / fileName;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		const HRESULT result = DirectX::CreateDDSTextureFromFile(&device, &context, texturePath.c_str(), nullptr, &shaderResourceView);
		WIN_CHECK_HRESULT(result);

		return shaderResourceView;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DX11Factory::LoadNonDDS(ID3D11Device& device, const std::filesystem::path& path)
	{
		int width, height, channels;
		const std::filesystem::path texturePath = std::filesystem::absolute(SIMPLE_DIR_TEXTURES) / path;
		unsigned char* img = stbi_load(texturePath.string().c_str(), &width, &height, &channels, 0);

		if (img == nullptr)
		{
			return nullptr;
		}

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;


		if (channels == 3)
		{
			std::vector<unsigned char> imageData(width * height * 4);

			for (int i = 0; i < width * height; ++i)
			{
				imageData[static_cast<size_t>(4 * i + 0)] = img[3 * i + 0];
				imageData[static_cast<size_t>(4 * i + 1)] = img[3 * i + 1];
				imageData[static_cast<size_t>(4 * i + 2)] = img[3 * i + 2];
				imageData[static_cast<size_t>(4 * i + 3)] = 255;
			}

			shaderResourceView = DX11Factory::CreateTexture(device, Vector2ui(width, height), imageData.data());
		}
		else if (channels == 4)
		{
			shaderResourceView = DX11Factory::CreateTexture(device, Vector2ui(width, height), img);
		}
		else
		{
			return nullptr;
		}

		return shaderResourceView;
	}
}


#endif