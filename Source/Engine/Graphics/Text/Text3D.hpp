#pragma once
#include "Engine/Math/Point3.hpp"
#include "Engine/Utility/Color.hpp"
#include <string>

namespace Simple
{

	class Text3D final
	{
	public:

		constexpr Text3D() = default;
		constexpr Text3D(const Point3f& position, std::string text, const Color& color = Colors::White, const float scale = 1.f);
		constexpr explicit Text3D(const Point3f& position);

		[[nodiscard]] constexpr const Point3f& GetPosition() const noexcept;
		constexpr void SetPosition(const Point3f& position) noexcept;

		[[nodiscard]] constexpr const std::string& GetText() const noexcept;
		constexpr void SetText(std::string&& text) noexcept;
		constexpr void SetText(const std::string& text) noexcept;

		[[nodiscard]] constexpr const Color& GetColor() const noexcept;
		constexpr void SetColor(const Color& color) noexcept;

		[[nodiscard]] constexpr float GetScale() const noexcept;
		constexpr void SetScale(float scale) noexcept;

	private:

		std::string mText;
		Point3f mPosition;
		Color mColor = Colors::White;
		float mScale = 1.f;
	};

	constexpr Text3D::Text3D(const Point3f& position, std::string text, const Color& color, const float scale)
		: mPosition(position)
		, mText(std::move(text))
		, mColor(color)
		, mScale(scale)
	{
	}

	constexpr Text3D::Text3D(const Point3f& position)
		: mPosition(position)
	{
	}

	constexpr const Point3f& Text3D::GetPosition() const noexcept
	{
		return mPosition;
	}

	constexpr void Text3D::SetPosition(const Point3f& position) noexcept
	{
		mPosition = position;
	}

	constexpr const std::string& Text3D::GetText() const noexcept
	{
		return mText;
	}

	constexpr void Text3D::SetText(std::string&& text) noexcept
	{
		mText = std::move(text);
	}

	constexpr void Text3D::SetText(const std::string& text) noexcept
	{
		mText = text;
	}

	constexpr const Color& Text3D::GetColor() const noexcept
	{
		return mColor;
	}

	constexpr void Text3D::SetColor(const Color& color) noexcept
	{
		mColor = color;
	}

	constexpr float Text3D::GetScale() const noexcept
	{
		return mScale;
	}

	constexpr void Text3D::SetScale(float scale) noexcept
	{
		mScale = scale;
	}
}