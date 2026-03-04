#pragma once

#ifdef _WIN32

#include <wrl/client.h>
#include <d3d11.h>
#include "Engine/Graphics/DX11/DX11Factory.hpp"

namespace CLX
{

	template<typename T> requires std::is_trivially_copyable_v<T>
	class DX11ConstantBuffer final
	{
	public:

		using type = T;

		DX11ConstantBuffer() = default;

		void Init(ID3D11Device& device, const unsigned int slot)
		{
			mBuffer = DX11Factory::CreateConstantBuffer(device, sizeof(T));
			mSlot = slot;
		}

		void UpdateAndBind(const T& data, ID3D11DeviceContext& context)
		{
			Update(data, context);
			Bind(context);
		}

		void Update(const T& data, ID3D11DeviceContext& context)
		{
			D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
			context.Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
			std::memcpy(mappedBuffer.pData, &data, sizeof(T));
			context.Unmap(mBuffer.Get(), 0);
		}

		void Bind(const unsigned int slot, ID3D11DeviceContext& context)
		{
			context.VSSetConstantBuffers(slot, 1, mBuffer.GetAddressOf());
			context.PSSetConstantBuffers(slot, 1, mBuffer.GetAddressOf());
		}

		void Bind(ID3D11DeviceContext& context)
		{
			context.VSSetConstantBuffers(mSlot, 1, mBuffer.GetAddressOf());
			context.PSSetConstantBuffers(mSlot, 1, mBuffer.GetAddressOf());
		}

		void SetSlot(const unsigned int slot)
		{
			mSlot = slot;
		}

		[[nodiscard]] unsigned int GetSlot() const
		{
			return mSlot;
		}

	private:

		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;
		unsigned int mSlot = 100;
	};
}

#endif