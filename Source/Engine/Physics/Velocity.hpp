#pragma once
#include "Utility/Math/Vector3.hpp"
#include "Utility/StrongType.hpp"

namespace Simple
{
	namespace Physics
	{
		template<typename T>
		using Velocity = StrongType<Vector3<T>, struct STVelocity>;

		template<typename T>
		struct ST_Scalar<Velocity<T>> : std::true_type {};

		using Velocityf = Velocity<float>;
	}
}