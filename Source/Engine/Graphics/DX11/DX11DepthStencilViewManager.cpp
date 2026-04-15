#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11DepthStencilViewManager.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"

namespace CLX
{

	DX11DepthStencilViewManager::DX11DepthStencilViewManager(Microsoft::WRL::ComPtr<ID3D11Device> device)
		: mDevice(std::move(device))
	{
	}

	DepthStencilViewHandle DX11DepthStencilViewManager::Create(const Dimension2u windowSize)
	{
		mDepthStencilViews[mIDCounter] = DX11Factory::CreateDepthStencilView(*mDevice.Get(), windowSize);

		unsigned int id = mIDCounter;
		mIDCounter++;

		return DepthStencilViewHandle(id);
	}

	void DX11DepthStencilViewManager::Initialize(const DepthStencilViewHandle handle, const Dimension2u windowSize)
	{
		auto dsv = Get(handle);
		if (dsv == nullptr)
		{
			return;
		}
		
		mDepthStencilViews[handle.GetID()] = DX11Factory::CreateDepthStencilView(*mDevice.Get(), windowSize);
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DX11DepthStencilViewManager::Get(DepthStencilViewHandle handle)
	{
		const auto it = mDepthStencilViews.find(handle.GetID());
		if (it != mDepthStencilViews.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void DX11DepthStencilViewManager::InitializeAll(const Dimension2u windowSize)
	{
		for (auto& depthStencilView : mDepthStencilViews)
		{
			depthStencilView.second = DX11Factory::CreateDepthStencilView(*mDevice.Get(), windowSize);
		}
	}
}