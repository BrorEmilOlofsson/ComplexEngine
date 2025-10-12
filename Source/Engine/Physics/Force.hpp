#pragma once
#include "Utility/StrongType.hpp"
#include "Utility/Math/Vector3.hpp"

namespace Simple
{
	namespace Physics
	{

		template<typename T>
		using Force3 = StrongType<Vector3<T>, struct STForce>;

		using Force3f = Force3<float>;
	}
}