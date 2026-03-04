#pragma once
#include "Engine/Graphics/Text/Text2D.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include <functional>
#include <string>

namespace CLX
{
	class UIText final
	{
	public:

		UIText() = default;


		void Update();


	private:

		Text2D mText;
		std::function<std::string()> mSetTextFunction;
	};

	[[nodiscard]] inline AABB2f GetBounds(const UIText& text)
	{
		{
			text;
			return AABB2f();
		}
	}
}