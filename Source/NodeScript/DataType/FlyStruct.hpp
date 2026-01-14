#pragma once
#include "../FlyDefines.hpp"
#include "FlyVariableContainer.hpp"
#include <string>

namespace FLY_NAMESPACE
{

	class Struct final
	{
	public:

		Struct() = default;
		Struct(std::string name);

		[[nodiscard]] std::string& Name();
		[[nodiscard]] const std::string& Name() const;

		[[nodiscard]] constexpr VariableContainer& GetVariableContainer();
		[[nodiscard]] constexpr const VariableContainer& GetVariableContainer() const;

	private:

		VariableContainer mVariableContainer;
		std::string mName;
	};

	constexpr VariableContainer& Struct::GetVariableContainer()
	{
		return mVariableContainer;
	}
	
	constexpr const VariableContainer& Struct::GetVariableContainer() const
	{
		return mVariableContainer;
	}
}