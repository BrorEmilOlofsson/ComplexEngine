#pragma once
#include <unordered_map>
#include "Engine/Win/WinDefines.hpp"

#include "Engine/Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Engine/Graphics/RenderTarget/RenderTargetView.hpp"

#ifdef _WIN32

namespace Simple
{

	class DX11RenderTargetManager final
	{
	public:

		DX11RenderTargetManager(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		[[nodiscard]] RenderTargetView Create(Vector2ui windowSize);

		[[nodiscard]] DX11RenderTarget* Get(RenderTargetView view);

		void ClearAll(Color color);

		[[nodiscard]] auto begin()
		{
			return mRenderTargets.begin();
		}

		[[nodiscard]] auto end()
		{
			return mRenderTargets.end();
		}

	private:

		std::unordered_map<unsigned int, DX11RenderTarget> mRenderTargets;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	};
}

#endif