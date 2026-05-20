#pragma once

#ifdef _WIN32


#define NOMINMAX

#include <unordered_map>
#include <d3d11.h>
#include <wrl/client.h>

#include "Engine/Graphics/DepthBuffer/DepthStencilViewHandle.hpp"
#include "Engine/Math/Dimension2.hpp"

namespace CLX
{

	class DX11DepthStencilViewManager final
	{
	public:

		explicit DX11DepthStencilViewManager(Microsoft::WRL::ComPtr<ID3D11Device> device);

		DepthStencilViewHandle Create(const Dimension2u windowSize);
		void Initialize(const DepthStencilViewHandle handle, const Dimension2u windowSize);

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Get(DepthStencilViewHandle handle);

		void InitializeAll(const Dimension2u windowSize);

		[[nodiscard]] auto begin() const { return mDepthStencilViews.begin(); }
		[[nodiscard]] auto end() const { return mDepthStencilViews.end(); }
	private:

		std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> mDepthStencilViews;
		unsigned int mIDCounter = 0;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice = nullptr;
	};
}

#endif