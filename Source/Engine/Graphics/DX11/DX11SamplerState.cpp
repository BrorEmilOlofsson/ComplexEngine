#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11SamplerState.hpp"
#include "Engine/Graphics/DX11/DX11Factory.hpp"

namespace Simple
{

	DX11SamplerState::DX11SamplerState(ID3D11Device& device)
		: mSamplerState(DX11Factory::CreateSamplerState(device))
	{
	}

	void DX11SamplerState::Bind(ID3D11DeviceContext& context)
	{
		context.VSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	}
}