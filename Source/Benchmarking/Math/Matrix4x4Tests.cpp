#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Math/Matrix4x4.hpp"

using namespace CLX;

Matrix4x4f GetRandomMatrix();
std::array<float, 16> GetRandomArray();

TEST_CASE("Benchmarking")
{
}

Matrix4x4f GetRandomMatrix()
{
	return Matrix4x4f(
		{
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
		}
		);
}

std::array<float, 16> GetRandomArray()
{
	return {
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
			static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),static_cast<float>(rand()),
	};
}