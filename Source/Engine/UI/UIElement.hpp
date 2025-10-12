#pragma once
#include <variant>
#include "UIButton.hpp"
#include "UIText.hpp"

namespace Simple
{
	using UIElement = std::variant<UIButton, UIText>;
}