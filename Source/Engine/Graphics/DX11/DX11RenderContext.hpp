#pragma once
#ifdef _WIN32

#include <vector>

#include <wrl/client.h>
#include <d3d11.h>

#include "Engine/Graphics/DX11/RenderTarget/DX11RenderTarget.hpp"
#include "Engine/Graphics/DX11/DX11GBuffer.hpp"
#include "Engine/Math/Dimension2.hpp"
#include "Engine/Math/Point2.hpp"

namespace CLX
{
	class DX11RenderContext final
	{
	public:
		
		DX11RenderContext(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Dimension2u size);

		[[nodiscard]] std::vector<void*> GetGBufferSRVs();
		[[nodiscard]] Dimension2u GetBufferSize() const;
		[[nodiscard]] uint32_t GetObjectIDAt(const Point2i& point);
		[[nodiscard]] DX11RenderTarget& GetOutputRenderTarget();
		[[nodiscard]] ID3D11ShaderResourceView* GetOutputSRV();
		[[nodiscard]] DX11GBuffer& GetGBuffer();

		void ResizeBuffers(const Dimension2u& size);
		void ClearBuffers();



		void SetOutputRenderTarget();
		void SetGBufferRenderTargets();
		void SetGBufferShaderResources();

	private:

		DX11GBuffer mGBuffer;
		DX11RenderTarget mOutputRT;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	};
}

#endif