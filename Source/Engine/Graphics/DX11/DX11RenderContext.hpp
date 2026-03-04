#pragma once

#ifdef _WIN32

#include "Engine/Graphics/DX11/DX11GBuffer.hpp"
#include <vector>

namespace CLX
{
	class DX11RenderContext final
	{
	public:
		
		DX11RenderContext(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Vector2ui size);

		[[nodiscard]] std::vector<void*> GetGBufferSRVs();
		[[nodiscard]] Vector2ui GetBufferSize() const;
		[[nodiscard]] uint32_t GetObjectIDAt(const Point2i& point);
		[[nodiscard]] DX11RenderTarget& GetOutputRenderTarget();
		[[nodiscard]] ID3D11ShaderResourceView* GetOutputSRV();
		[[nodiscard]] DX11GBuffer& GetGBuffer();

		void ResizeBuffers(const Vector2ui& size);
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