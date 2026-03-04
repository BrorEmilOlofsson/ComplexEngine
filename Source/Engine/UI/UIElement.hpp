#pragma once
#include <variant>
#include "UIButton.hpp"
#include "UIText.hpp"

namespace CLX
{
	using UIElement = std::variant<UIButton, UIText>;
}