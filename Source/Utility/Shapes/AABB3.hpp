#pragma once
#include "Utility/Bounds.hpp"
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/VectorMath.hpp"
#include <limits>
#include <span>
#include <array>

#undef min
#undef max

namespace Simple
{

	struct AABB3BoundsChecker final
	{
		template<typename T>
		[[nodiscard]] constexpr bool operator()(const Point3<T>& min, const Point3<T>& max) const
		{
			return min.x <= max.x && min.y <= max.y && min.z <= max.z;
		}
	};
	
	template<typename T>
	using AABB3 = Bounds<Point3<T>, AABB3BoundsChecker>;

	using AABB3f = AABB3<float>;
	using AABB3i = AABB3<int>;

	template<std::ranges::range R>
	[[nodiscard]] constexpr auto CreateAABB3FromPoints(const R& aPoints)
	{
		using T = typename std::remove_cvref_t<decltype(aPoints[0])>::value_type;
		Point3<T> min(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
		Point3<T> max(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest());

		for (const auto& p : aPoints)
		{
			min = Min(min, p);
			max = Max(max, p);
		}

		return AABB3<T>::FromMinAndMax(min, max);
	}

	template<std::ranges::range R, typename TransformF>
	[[nodiscard]] constexpr auto CreateAABB3FromPoints(const R& range, TransformF&& transformFunc)
	{
		using T = typename decltype(transformFunc(range[0]))::value_type;
		Point3<T> min(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
		Point3<T> max(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest());

		for (const auto& i : range)
		{
			const Point3<T> p = transformFunc(i);
			min = Min(min, p);
			max = Max(max, p);
		}

		return AABB3<T>::FromMinAndMax(min, max);
	}

	template<typename T>
	[[nodiscard]] constexpr AABB3<T> MinMax(const AABB3<T>& a, const AABB3<T>& b)
	{
		return AABB3<T>::FromMinAndMax(Min(a.GetMin(), b.GetMin()), Max(a.GetMax(), b.GetMax()));
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInside(const Point3<T>& point, const Point3<T>& min, const Point3<T>& max) noexcept
	{
		return !(point.x < min.x || point.y < min.y || point.z < min.z
			|| point.x > max.x || point.y > max.y || point.z > max.z);
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInside(const Point3<T>& point, const AABB3<T>& aabb) noexcept
	{
		return IsInside(point, aabb.GetMin(), aabb.GetMax());
	}

	template<typename T, typename ContainerT = std::array<Point3<T>, 8>>
	[[nodiscard]] constexpr ContainerT GetCorners(const AABB3<T>& aabb)
	{
		const Point3<T> min = aabb.GetMin();
		const Point3<T> max = aabb.GetMax();
		return ContainerT
		{
			Point3<T>(min.x, min.y, min.z),
			Point3<T>(max.x, min.y, min.z),
			Point3<T>(max.x, max.y, min.z),
			Point3<T>(min.x, max.y, min.z),
			Point3<T>(min.x, min.y, max.z),
			Point3<T>(max.x, min.y, max.z),
			Point3<T>(max.x, max.y, max.z),
			Point3<T>(min.x, max.y, max.z)
		};
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const AABB3<T>& aabb)
	{
		os << "Min: " << aabb.GetMin() << ", Max: " << aabb.GetMax();
		return os;
	}
}