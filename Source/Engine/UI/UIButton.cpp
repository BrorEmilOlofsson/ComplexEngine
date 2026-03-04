#include "Engine/Precompiled/EnginePch.hpp"
#include "UIButton.hpp"

namespace CLX
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