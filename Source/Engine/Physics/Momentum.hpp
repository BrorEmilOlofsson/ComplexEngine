#pragma once
#include "Utility/StrongType.hpp"
#include "Utility/Math/Vector3.hpp"

namespace Simple
{
	namespace Physics
	{
		template<typename T>
		using Momentum3 = StrongType<Vector3<T>, struct STMomentum>;

		using Momentum3f = Momentum3<float>;

		template<typename T>
		auto Negate(Momentum3<T>) -> Momentum3<T>;
	}
}