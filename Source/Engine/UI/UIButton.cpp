#include "Engine/Precompiled/EnginePch.hpp"
#include "UIButton.hpp"

namespace Simple
{
	void UIButton::Update()
	{
		mText.Update();
	}

	void UIButton::OnClick()
	{
		if (mOnClickFunction)
		{
			mOnClickFunction();
		}
	}
}