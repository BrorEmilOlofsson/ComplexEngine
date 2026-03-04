#pragma once
#include "Engine/Math/Point2.hpp"
#include "Engine/Utility/Color.hpp"
#include <string>

namespace CLX
{

	class Text2D final
	{
	public:

		constexpr Text2D() = default;
		constexpr Text2D(const Point2f& position, std::string text, const Color& color = Colors::White, const float scale = 1.f);
		constexpr explicit Text2D(const Point2f& position);

		[[nodiscard]] constexpr const Point2f& GetPosition() const noexcept;
		constexpr void SetPosition(const Point2f& position) noexcept;

		[[nodiscard]] constexpr const std::string& GetText() const noexcept;
		constexpr void SetText(std::string&& text) noexcept;
		constexpr void SetText(const std::string& text);

		[[nodiscard]] constexpr const Color& GetColor() const noexcept;
		constexpr void SetColor(const Color& color) noexcept;

		[[nodiscard]] constexpr float GetScale() const noexcept;
		constexpr void SetScale(float scale) noexcept;

	private:

		std::string mText;
		Point2f mPosition;
		Color mColor = Colors::White;
		float mScale = 1.f;
	};

	constexpr Text2D::Text2D(const Point2f& position, std::string text, const Color& color, const float scale)
		: mPosition(position)
		, mText(std::move(text))
		, mColor(color)
		, mScale(scale)
	{
	}

	constexpr Text2D::Text2D(const Point2f& position)
		: mPosition(position)
	{
	}

	constexpr const Point2f& Text2D::GetPosition() const noexcept
	{
		return mPosition;
	}

	constexpr void Text2D::SetPosition(const Point2f& position) noexcept
	{
		mPosition = position;
	}

	constexpr const std::string& Text2D::GetText() const noexcept
	{
		return mText;
	}

	constexpr void Text2D::SetText(std::string&& text) noexcept
	{
		mText = std::move(text);
	}

	constexpr void Text2D::SetText(const std::string& text)
	{
		mText = text;
	}

	constexpr const Color& Text2D::GetColor() const noexcept
	{
		return mColor;
	}

	constexpr void Text2D::SetColor(const Color& color) noexcept
	{
		mColor = color;
	}

	constexpr float Text2D::GetScale() const noexcept
	{
		return mScale;
	}

	constexpr void Text2D::SetScale(float scale) noexcept
	{
		mScale = scale;
	}
}