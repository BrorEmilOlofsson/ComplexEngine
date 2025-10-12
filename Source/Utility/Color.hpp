#pragma once
#include "Utility/RGBColor.hpp"

namespace Simple
{

	class Color final
	{
	public:

		constexpr Color() = default;
		constexpr Color(const float r, const float g, const float b, const float a = 1.f)
			: r(r)
			, g(g)
			, b(b)
			, a(a)
		{
		}

		constexpr Color(const RGBColor& color, const float a = 1.f)
			: r(color.r)
			, g(color.g)
			, b(color.b)
			, a(a)
		{

		}

		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 1.f;
	};

	[[nodiscard]] constexpr Color operator*(const Color& color, const float scalar) noexcept
	{
		return Color(color.r * scalar, color.g * scalar, color.b * scalar, color.a * scalar);
	}

	[[nodiscard]] constexpr Color operator*(const Color& color1, const Color& color2) noexcept
	{
		return Color(color1.r * color2.r, color1.g * color2.g, color1.b * color2.b, color1.a * color2.a);
	}

	[[nodiscard]] constexpr bool operator==(const Color& a, const Color& b) noexcept
	{
		return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
	}
}