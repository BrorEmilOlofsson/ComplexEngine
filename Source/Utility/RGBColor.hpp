#pragma once

namespace Simple
{

	class RGBColor final
	{
	public:

		constexpr RGBColor() = default;
		constexpr RGBColor(const float r, const float g, const float b) noexcept
			: r(r)
			, g(g)
			, b(b)
		{
		}

		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
	};

	namespace Colors
	{
		constexpr RGBColor Black = RGBColor(0.f, 0.f, 0.f);
		constexpr RGBColor Red = RGBColor(1.f, 0.f, 0.f);
		constexpr RGBColor Green = RGBColor(0.f, 1.f, 0.f);
		constexpr RGBColor Blue = RGBColor(0.f, 0.f, 1.f);
		constexpr RGBColor Yellow = RGBColor(1.f, 1.f, 0.f);
		constexpr RGBColor White = RGBColor(1.f, 1.f, 1.f);
		constexpr RGBColor Pink = RGBColor(1.f, 0.1f, 0.5f);
		constexpr RGBColor Purple = RGBColor(1.f, 0.1f, 0.8f);
		constexpr RGBColor Orange = RGBColor(1.f, 0.5f, 0.1f);
		constexpr RGBColor Magenta = RGBColor(1.f, 0.f, 1.f);
		constexpr RGBColor Aqua = RGBColor(0.f, 1.f, 1.f);
		constexpr RGBColor Cyan = RGBColor(0.f, 0.75f, 1.f);
		constexpr RGBColor Teal = RGBColor(0.f, 0.5f, 0.5f);
		constexpr RGBColor Navy = RGBColor(0.f, 0.f, 0.5f);
		constexpr RGBColor Maroon = RGBColor(0.5f, 0.f, 0.f);
		constexpr RGBColor Olive = RGBColor(0.5f, 0.5f, 0.f);
		constexpr RGBColor Lime = RGBColor(0.5f, 1.f, 0.f);
		constexpr RGBColor Gold = RGBColor(1.f, 0.84f, 0.f);
		constexpr RGBColor Silver = RGBColor(0.75f, 0.75f, 0.75f);
		constexpr RGBColor Gray = RGBColor(0.5f, 0.5f, 0.5f);
		constexpr RGBColor LightGray = RGBColor(0.8f, 0.8f, 0.8f);
		constexpr RGBColor DarkGray = RGBColor(0.2f, 0.2f, 0.2f);
		constexpr RGBColor Brown = RGBColor(0.6f, 0.3f, 0.f);
		constexpr RGBColor Beige = RGBColor(0.96f, 0.96f, 0.86f);
		constexpr RGBColor SkyBlue = RGBColor(0.53f, 0.81f, 0.92f);
		constexpr RGBColor Coral = RGBColor(1.f, 0.5f, 0.31f);
		constexpr RGBColor Salmon = RGBColor(0.98f, 0.5f, 0.45f);
		constexpr RGBColor Indigo = RGBColor(0.29f, 0.f, 0.51f);
		constexpr RGBColor Mint = RGBColor(0.6f, 1.f, 0.6f);
		constexpr RGBColor BritishRacingGreen = RGBColor(0.0f, 0.26f, 0.15f);
		constexpr RGBColor LightBlue = RGBColor(0.68f, 0.85f, 0.9f);
		constexpr RGBColor LightGreen = RGBColor(0.56f, 0.93f, 0.56f);
		constexpr RGBColor LightPink = RGBColor(1.f, 0.71f, 0.76f);
		constexpr RGBColor LightPurple = RGBColor(0.8f, 0.52f, 0.8f);
		constexpr RGBColor LightOrange = RGBColor(1.f, 0.65f, 0.4f);
		constexpr RGBColor LightMagenta = RGBColor(1.f, 0.4f, 1.f);
		constexpr RGBColor CafeLatte = RGBColor(0.96f, 0.87f, 0.7f);
		constexpr RGBColor Cardinal = RGBColor(0.89f, 0.17f, 0.19f);
		constexpr RGBColor Charcoal = RGBColor(0.11f, 0.11f, 0.11f);
		constexpr RGBColor FrenchRose = RGBColor(1.f, 0.27f, 0.51f);
		constexpr RGBColor DukeBlue = RGBColor(0, 0, 0.411764705882f);
	}
}