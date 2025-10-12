#pragma once
#include <vector>
#include "UILayer.hpp"

namespace Simple
{

	class UICanvas final
	{
	public:

		UICanvas() = default;

		void Update();
		[[nodiscard]] std::optional<const UIElement*> GetElementAt(const Point2f& position) const;

	private:

		std::vector<UILayer> mLayers;
	};
}