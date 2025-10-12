#pragma once
#include "StrongType.hpp"
#include <ratio>

template<typename T>
using Length = StrongType<T, struct STLength>;

template<typename T>
using Meter = StrongType<T, struct STMeter>;


template<typename T>
auto Add(Meter<T>, Meter<T>) -> Meter<T>;
template<typename T>
auto Subtract(Meter<T>, Meter<T>) -> Meter<T>;
template<typename T>
auto AddEqual(Meter<T>, Meter<T>) -> Meter<T>;
template<typename T>
auto SubtractEqual(Meter<T>, Meter<T>) -> Meter<T>;
template<typename T>
auto Multiply(Meter<T>, T) -> Meter<T>;

template<typename T>
using Centimeter = StrongType<T, struct STCentimeter>;


template<typename T>
auto Add(Centimeter<T>, Centimeter<T>) -> Centimeter<T>;
template<typename T>
auto Subtract(Centimeter<T>, Centimeter<T>) -> Centimeter<T>;
template<typename T>
auto AddEqual(Centimeter<T>, Centimeter<T>) -> Centimeter<T>;
template<typename T>
auto SubtractEqual(Centimeter<T>, Centimeter<T>) -> Centimeter<T>;
template<typename T>
auto Multiply(Centimeter<T>, T) -> Centimeter<T>;