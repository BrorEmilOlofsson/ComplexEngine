#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11RenderContext.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"

namespace CLX
{

	DX11RenderContext::DX11RenderContext(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Dimension2u size)
		: mGBuffer(context, device, size)
		, mOutputRT(context, device, *DX11Factory::CreateRenderTargetTexture(*device.Get(), DX11Factory::CreateRenderTargetTextureDesc(size)).Get(), size)
		, mDevice(device)
		, mContext(context)
	{
	}


	std::vector<void*> DX11RenderContext::GetGBufferSRVs()
	{
		auto srvArray = mGBuffer.GetSRVArray();

		std::vector<void*> srvs;
		srvs.reserve(srvArray.size());
		std::ranges::transform(srvArray, std::back_inserter(srvs), [](auto* srvPtr) { return (void*)srvPtr; });
		return srvs;
	}

	Dimension2u DX11RenderContext::GetBufferSize() const
	{
		return mGBuffer.GetSize();
	}

	static uint32_t ReconstructObjectID(ID3D11DeviceContext& context, ID3D11Texture2D& stagingTexture, ID3D11Texture2D& idTexture, Point2i pos)
	{

		// Copy GPU texture to a staging resource
		context.CopyResource(&stagingTexture, &idTexture);
		PROFILER_END();

		D3D11_MAPPED_SUBRESOURCE mapped{};
		context.Map(&stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);

		uint32_t* pixels = (uint32_t*)mapped.pData;
		uint32_t id = pixels[pos.y * (mapped.RowPitch / sizeof(uint32_t)) + pos.x];

		context.Unmap(&stagingTexture, 0);
		return id;
	}

	uint32_t DX11RenderContext::GetObjectIDAt(const Point2i& pos)
	{
		auto stagingTexture = DX11Factory::CreateObjectIDStagingTexture(*mDevice.Get(), mGBuffer.GetSize());
		return ReconstructObjectID(*mContext.Get(), *stagingTexture.Get(), *mGBuffer.mObjectIDTexture.Get(), pos);
	}

	DX11RenderTarget& DX11RenderContext::GetOutputRenderTarget()
	{
		return mOutputRT;
	}

	ID3D11ShaderResourceView* DX11RenderContext::GetOutputSRV()
	{
		return mOutputRT.GetShaderResourceView();
	}

	DX11GBuffer& DX11RenderContext::GetGBuffer()
	{
		return mGBuffer;
	}

	void DX11RenderContext::ResizeBuffers(const Dimension2u& size)
	{
		mGBuffer.Resize(size);
		auto texture = DX11Factory::CreateRenderTargetTexture(*mDevice.Get(), size);
		mOutputRT.Resize(*texture.Get(), size);
	}

	void DX11RenderContext::ClearBuffers()
	{
		mGBuffer.Clear();
		mOutputRT.Clear(Colors::Black);
	}

	void DX11RenderContext::SetOutputRenderTarget()
	{
		mOutputRT.Set();
	}

	void DX11RenderContext::SetGBufferRenderTargets()
	{
		mGBuffer.SetRenderTargets();
	}

	void DX11RenderContext::SetGBufferShaderResources()
	{
		mGBuffer.SetShaderResources();
	}

}