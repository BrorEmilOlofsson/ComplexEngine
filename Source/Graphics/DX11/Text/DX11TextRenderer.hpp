#pragma once
#include <memory>
#include <string>
#include <span>
#include "Graphics/Text/Text3D.hpp"

#ifdef _WIN32

#include <External/DirectXTK/SpriteBatch.h>
#include <External/DirectXTK/SpriteFont.h>

namespace Simple
{

	class DX11TextRenderer final
	{
	public:

		DX11TextRenderer() = default;

		void Init(ID3D11DeviceContext* context, ID3D11Device* device, const std::wstring& font);

		void Render(std::span<const Text3D> texts, const Camera& camera, const Vector2ui& windowSize);

	private:

		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
	};
}

#endif