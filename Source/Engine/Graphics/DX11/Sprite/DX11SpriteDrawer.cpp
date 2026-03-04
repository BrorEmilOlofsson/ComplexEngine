#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11SpriteDrawer.hpp"
#include "Engine/Utility/Win/WinAdapterFunctions.hpp"
#include "Engine/Math/VectorMath.hpp"
#include "Engine/Graphics/Texture/Texture.hpp"

namespace Simple
{

	void DX11SpriteDrawer::Init(ID3D11Device& device, ID3D11DeviceContext& deviceContext, TextureAssetHandle texture)
	{
		mCommonStates = std::make_unique<DirectX::CommonStates>(&device);
		mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(&deviceContext);
		SetDefaultTexture(texture);
	}

	void DX11SpriteDrawer::SetDefaultTexture(TextureAssetHandle texture)
	{
		mDefaultTexture = texture;
	}

	void DX11SpriteDrawer::Render(const std::span<const Sprite2D> sprites, ID3D11DeviceContext& context, const Camera& camera)
	{
		for (const auto& sprite : sprites)
		{
			Render(sprite, context);
		}

		camera;
	}

	void DX11SpriteDrawer::Render(const Sprite2D& sprite, ID3D11DeviceContext& context)
	{
		TextureAssetHandle texture;
		if (sprite.GetTexture())
		{
			texture = sprite.GetTexture();
		}
		else if (mDefaultTexture)
		{
			texture = mDefaultTexture;
		}
		else
		{
			return;
		}

		context.OMSetDepthStencilState(nullptr, 0);

		mSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, mCommonStates->NonPremultiplied());

		texture->Bind();

		const Vector2f pivot = sprite.GetPivot();
		//pivot.x *= aSprite->GetPixelSize().x;
		//pivot.y *= aSprite->GetPixelSize().y;

		const RECT uvRect = ToRECT(sprite.GetUVAABB());

		mSpriteBatch->Draw(
			(ID3D11ShaderResourceView*)texture->GetShaderResourceView(), // texture
			ToXMFloat2(sprite.GetPosition()), // position
			&uvRect, // UV RECT
			ToXMVector(sprite.GetColor()), // Color
			sprite.GetRotation().Value(), // Rotation
			ToXMFloat2(sprite.GetPivot()), // Pivot
			ToXMFloat2(sprite.GetScale()), // Scale
			DirectX::SpriteEffects_None, // SpriteEffect Flip X,Y
			sprite.GetLayerDepth() // layerdepth
		);

		mSpriteBatch->End();
	}

	void DX11SpriteDrawer::Render(const Sprite3D& sprite, ID3D11DeviceContext& context, const Camera& camera)
	{
		sprite;
		context;
		camera;
		/*const Texture* texture = nullptr;
		if (aSprite.myTexture != nullptr)
		{
			texture = aSprite.myTexture;
		}
		else if (myDefaultTexture != nullptr)
		{
			texture = myDefaultTexture;
		}

		if (texture == nullptr)
		{
			return;
		}



		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		myShader->BindThisShader(context);

		context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		myTransformBuffer.Bind(context);

		for (const auto& mesh : aDrawList)
		{
			for (Vertex& vertex : myVertices)
			{
				vertex.color = mesh.color;
			}
			context->UpdateSubresource(myVertexBuffer.Get(), 0, nullptr, myVertices.data(), 0, 0);


			TransformBufferData objectBuffer
			{
				.modelWorldMatrix = aGetTransformMatrixFunction(mesh)
			};

			myTransformBuffer.Update(sizeof(TransformBufferData), &objectBuffer, context);

			context->DrawIndexed(static_cast<UINT>(myIndices.size()), 0, 0);
		}*/

	}
}