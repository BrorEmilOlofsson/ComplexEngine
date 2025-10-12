#pragma once
#include <cmath>
#include <random>
#include "Math.hpp"

template<std::floating_point T>
inline T Gaussian(const T pX)
{
	return std::pow(Napier<T>, std::pow(pX, T{ 2 }) * -1);
}


template<std::floating_point T>
constexpr T GenerateNormalRandomValue(T pMean = 0.0, T pStdDev = 1.0)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<T> dist(pMean, pStdDev);

	return dist(gen);
}