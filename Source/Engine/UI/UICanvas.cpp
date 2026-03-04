#include "Engine/Precompiled/EnginePch.hpp"
#include "UICanvas.hpp"

namespace CLX
{

	void UICanvas::Update()
	{
		for (UILayer& layer : mLayers)
		{
			layer.Update();
		}
	}

	std::optional<const UIElement*> UICanvas::GetElementAt(const Point2f& position) const
	{
		for (const UILayer& layer : mLayers)
		{
			if (auto element = layer.GetElementAt(position))
			{
				return element;
			}
		}

		return std::nullopt;
	}
}
