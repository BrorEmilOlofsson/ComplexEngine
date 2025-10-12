#pragma once
#include "Engine/Physics/Acceleration.hpp"
#include "Utility/StrongType.hpp"

namespace Simple
{
	namespace Physics
	{

		template<typename T>
		using Gravity = StrongType<T, struct STGravity>;

		template<typename T>
		constexpr Gravity<T> EARTH_GRAVITY = Gravity<T>(-9.81f);

		template<typename T>
		constexpr Acceleration<T> EARTH_GRAVITY_ACCELERATION = Acceleration<T>(Vector3<T>(0.f, EARTH_GRAVITY<T>.Value(), 0.f));

	}
}