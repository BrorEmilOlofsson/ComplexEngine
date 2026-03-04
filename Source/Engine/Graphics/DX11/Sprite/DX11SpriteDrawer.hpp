#pragma once
#include "Engine/Graphics/Sprite/Sprite2D.hpp"
#include "Engine/Graphics/Sprite/Sprite3D.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"
#include <memory>

#ifdef _WIN32

#include <External/DirectXTK/CommonStates.h>
#include <External/DirectXTK/SpriteBatch.h>

namespace Simple
{

	class DX11SpriteDrawer final
	{
	public:

		DX11SpriteDrawer() = default;

		void Init(ID3D11Device& device, ID3D11DeviceContext& context, TextureAssetHandle texture);

		void SetDefaultTexture(TextureAssetHandle defaultTexture);

		void Render(std::span<const Sprite2D> sprites, ID3D11DeviceContext& context, const Camera& camera);
		void Render(const Sprite2D& sprite, ID3D11DeviceContext& context);
		void Render(const Sprite3D& sprite, ID3D11DeviceContext& context, const Camera& camera);

	private:

		std::unique_ptr<DirectX::CommonStates> mCommonStates;
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

		TextureAssetHandle mDefaultTexture;

		DX11ConstantBuffer<TransformBufferData> mTransformBuffer;
	};
}

#endif