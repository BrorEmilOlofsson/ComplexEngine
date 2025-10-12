#include "Engine/Precompiled/EnginePch.hpp"
#include "UIText.hpp"

namespace Simple
{
	void UIText::Update()
	{
		if (mSetTextFunction)
		{
			mText.SetText(mSetTextFunction());
		}
	}
}