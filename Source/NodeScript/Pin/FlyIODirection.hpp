#pragma once
#include "../FlyDefines.hpp"
#include <string_view>
#include <optional>

namespace FLY_NAMESPACE
{

	enum class eIODirection
	{
		Input,
		Output
	};

	[[nodiscard]] constexpr eIODirection InvertIODirection(const eIODirection ioDirection)
	{
		return ioDirection == eIODirection::Input ? eIODirection::Output : eIODirection::Input;
	}

	[[nodiscard]] constexpr decltype(auto) SelectByIODirection(eIODirection ioDirection, auto&& inputValue, auto&& outputValue)
	{
		return ioDirection == eIODirection::Input ? inputValue : outputValue;
	}

	[[nodiscard]] constexpr decltype(auto) SelectByIODirection(eIODirection ioDirection, const auto& inputValue, const auto& outputValue)
	{
		return ioDirection == eIODirection::Input ? inputValue : outputValue;
	}

	[[nodiscard]] constexpr std::string_view PinIODirectionToString(const eIODirection ioDirection)
	{
		return SelectByIODirection(ioDirection, std::string_view("Input"), std::string_view("Output"));
	}

	[[nodiscard]] constexpr std::optional<eIODirection> StringToPinIODirection(const std::string_view name)
	{
		if (name == "Input")
		{
			return eIODirection::Input;
		}
		else if (name == "Output")
		{
			return eIODirection::Output;
		}

        return std::nullopt;
	}
}