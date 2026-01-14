#pragma once
#include "../FlyDefines.hpp"
#include "FlyVariableContainerInstance.hpp"

namespace FLY_NAMESPACE
{

	class Struct;

	class StructInstance final
	{
	public:

		StructInstance() = default;
		StructInstance(const Struct& s);

		[[nodiscard]] constexpr const Struct* GetStruct() const;
		[[nodiscard]] constexpr operator bool() const;

		constexpr friend bool operator==(const StructInstance& lhs, const StructInstance& rhs);


	private:

		const Struct* mStruct = nullptr;

	public:

		VariableContainerInstance mVariableContainerInstance;
	};

    [[nodiscard]] constexpr const Struct* StructInstance::GetStruct() const
	{
		return mStruct;
	}

    constexpr StructInstance::operator bool() const
	{
		return mStruct != nullptr;
	}

	constexpr bool operator==(const StructInstance& lhs, const StructInstance& rhs)
	{
		if (lhs.mStruct != rhs.mStruct)
		{
			return true;
		}

		return lhs.mVariableContainerInstance == rhs.mVariableContainerInstance;
	}
}