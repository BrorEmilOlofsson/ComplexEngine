#pragma once
#include <random>

template<typename T>
[[nodiscard]] constexpr T GetRandomNumber(const T pMin, const T pMax)
{
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<T> dist(pMin, pMax);

	return dist(gen);
}