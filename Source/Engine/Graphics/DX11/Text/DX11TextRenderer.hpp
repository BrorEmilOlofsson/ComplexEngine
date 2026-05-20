#pragma once
#ifdef _WIN32

#define NOMINMAX

#include <memory>
#include <string>
#include <span>

#include <External/DirectXTK/SpriteBatch.h>
#include <External/DirectXTK/SpriteFont.h>

#include <d3d11.h>

#include "Engine/Graphics/Text/Text3D.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Math/Dimension2.hpp"

namespace CLX
{

	class DX11TextRenderer final
	{
	public:

		DX11TextRenderer() = default;

		void Init(ID3D11DeviceContext* context, ID3D11Device* device, const std::wstring& font);

		void Render(std::span<const Text3D> texts, const Camera& camera, const Dimension2u& windowSize);

	private:

		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
	};
}

#endif