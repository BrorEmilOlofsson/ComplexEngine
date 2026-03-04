#pragma once

#ifdef _WIN32

#include <wrl/client.h>
#include <d3d11.h>

namespace CLX
{
	
	class DX11SamplerState final
	{
	public:

		explicit DX11SamplerState(ID3D11Device& device);

		void Bind(ID3D11DeviceContext& context);

	private:

		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	};
}

#endif