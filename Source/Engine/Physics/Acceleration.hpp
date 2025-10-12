#pragma once
#include "Utility/StrongType.hpp"
#include "Utility/Math/Vector3.hpp"

namespace Simple
{
	namespace Physics
	{
		template<typename T>
		using Acceleration = StrongType<Vector3<T>, struct STAcceleration>;

		using Accelerationf = Acceleration<float>;

		template<typename T>
		auto Negate(Acceleration<T>) -> Acceleration<T>;
	}
}