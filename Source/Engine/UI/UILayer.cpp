#include "Engine/Precompiled/EnginePch.hpp"
#include "UILayer.hpp"
#include "Utility/Visitor.hpp"

namespace Simple
{

	void UILayer::Update()
	{
		for (UIElement& element : mElements)
		{
			std::visit([](auto& e) { e.Update(); }, element);
		}
	}
	std::span<UIElement> UILayer::GetElements()
	{
		return mElements;
	}

	const std::span<const UIElement> UILayer::GetElements() const
	{
		return mElements;
	}

	std::optional<const UIElement*> UILayer::GetElementAt(const Point2f& position) const
	{
		auto it = std::ranges::find_if(mElements, [position](const UIElement& element)
		{
			return std::visit([position](const auto& e)
				{
					return IsInside(position, GetBounds(e));
				}, element);
			});

		if (it != mElements.end())
		{
			return &(*it);
		}

		return std::nullopt;
	}
}
