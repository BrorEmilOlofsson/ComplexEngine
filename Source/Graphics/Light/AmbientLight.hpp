#pragma once
#include "Utility/RGBColor.hpp"

namespace Simple
{

	class AmbientLight final
	{
	public:

		constexpr AmbientLight() = default;

		constexpr AmbientLight(RGBColor color, float intensity = 1.f) noexcept
			: mColor(color)
			, mIntensity(intensity)
		{
		}

		[[nodiscard]] const RGBColor& GetColor() const noexcept
		{
			return mColor;
		}

		void SetColor(const RGBColor& color) noexcept
		{
			mColor = color;
		}

		[[nodiscard]] float GetIntensity() const noexcept
		{
			return mIntensity;
		}

		void SetIntensity(float intensity) noexcept
		{
			mIntensity = intensity;
		}

	private:

		RGBColor mColor = Colors::DarkGray;
		float mIntensity = 1.f;
	};
}