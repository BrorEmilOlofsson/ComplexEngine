#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11RenderTargetManager.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"

#ifdef _WIN32

namespace CLX
{
	DX11RenderTargetManager::DX11RenderTargetManager(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
		: mDevice(std::move(device))
		, mContext(std::move(context))
	{
	}

	RenderTargetView DX11RenderTargetManager::Create(Vector2ui size)
	{
		static unsigned int nextID = 1; // Start from 1 to avoid zero ID
		auto texture = DX11Factory::CreateRenderTargetTexture(*mDevice.Get(), DX11Factory::CreateRenderTargetTextureDesc(size));
		DX11RenderTarget newRenderTarget(mContext, mDevice, *texture.Get(), size);
		mRenderTargets.emplace(nextID, std::move(newRenderTarget));

		auto id = nextID;
		nextID++;
		return RenderTargetView(mRenderTargets.at(id), id);
	}
	
	DX11RenderTarget* DX11RenderTargetManager::Get(RenderTargetView view)
	{
		auto it = mRenderTargets.find(view.GetID());
		if (it == mRenderTargets.end())
		{
			return nullptr; // Render target not found
		}

		return &it->second;
	}

	void DX11RenderTargetManager::ClearAll(Color color)
	{
		for (auto& [_, renderTarget] : mRenderTargets)
		{
			renderTarget.Clear(color);
		}
	}
}

#endif