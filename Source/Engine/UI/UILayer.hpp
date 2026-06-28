#pragma once
#include <optional>
#include <vector>
#include <span>
#include "UIElement.hpp"

namespace CLX
{
	class UILayer final
	{
	public:

		UILayer() = default;

		void Update();

		[[nodiscard]] std::span<UIElement> GetElements();
		[[nodiscard]] const std::span<const UIElement> GetElements() const;

		[[nodiscard]] std::optional<const UIElement*> GetElementAt(const Point2f& position) const;

	private:

		std::vector<UIElement> mElements;
	};
}