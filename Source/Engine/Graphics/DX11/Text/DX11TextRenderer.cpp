#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11TextRenderer.hpp"
#include "Engine/Utility/Win/WinAdapterFunctions.hpp"
#include "Engine/Utility/CameraMath.hpp"

namespace Simple
{

	void DX11TextRenderer::Init(ID3D11DeviceContext* context, ID3D11Device* device, const std::wstring& font)
	{
		mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
		mSpriteFont = std::make_unique<DirectX::SpriteFont>(device, font.c_str());
	}

	void DX11TextRenderer::Render(std::span<const Text3D> texts, const Camera& camera, const Vector2ui& windowSize)
	{
		if (texts.empty())
		{
			return;
		}
		mSpriteBatch->Begin();
		for (const Text3D& text : texts)
		{
			const std::optional<Point2f> screenPos = GetScreenPosition(text.GetPosition(), camera, windowSize);

			if (!screenPos)
			{
				continue;
			}
			// Draw your string
			mSpriteFont->DrawString(mSpriteBatch.get(), text.GetText().c_str(), ToXMFloat2(screenPos.value()), ToXMVector(text.GetColor()), 0.f, ToXMFloat2(Point2f::Zero()), text.GetScale());

		}
		mSpriteBatch->End();
	}
}
