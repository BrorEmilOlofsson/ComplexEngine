#pragma once
#include "Utility/StrongType.hpp"

namespace Simple
{
	namespace Physics
	{
		template<typename T>
		using Mass = StrongType<T, struct STMass>;

		template<typename T>
		struct ST_Scalar<Mass<T>> : std::true_type {};

		using Massf = Mass<float>;
	}
}