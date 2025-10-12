#pragma once
#include "Graphics/DepthBuffer/DepthStencilViewHandle.hpp"
#include <unordered_map>

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>

namespace Simple
{

	class DX11DepthStencilViewManager final
	{
	public:

		explicit DX11DepthStencilViewManager(Microsoft::WRL::ComPtr<ID3D11Device> device);

		DepthStencilViewHandle Create(const Vector2ui windowSize);
		void Initialize(const DepthStencilViewHandle handle, const Vector2ui windowSize);

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Get(DepthStencilViewHandle handle);

		void InitializeAll(const Vector2ui windowSize);

		[[nodiscard]] auto begin() const { return mDepthStencilViews.begin(); }
		[[nodiscard]] auto end() const { return mDepthStencilViews.end(); }
	private:

		std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> mDepthStencilViews;
		unsigned int mIDCounter = 0;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice = nullptr;
	};
}

#endif