#pragma once
#include "Engine/Math/Shapes/AABB2.hpp"
#include "UIText.hpp"

namespace Simple
{

	class UIButton final
	{
	public:

		UIButton() = default;

		void Update();
		void OnClick();

		[[nodiscard]] friend AABB2f GetBounds(const UIButton& button)
		{
			return button.mBounds;
		}

		[[nodiscard]] const UIText& GetText() const
		{
			return mText;
		}

	private:

		AABB2f mBounds;
		UIText mText;
		std::function<void()> mOnClickFunction;
	};
}