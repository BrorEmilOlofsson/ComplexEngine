#include "Engine/Precompiled/EnginePch.hpp"
#include "UIText.hpp"

namespace CLX
{
	void UIText::Update()
	{
		if (mSetTextFunction)
		{
			mText.SetText(mSetTextFunction());
		}
	}
}