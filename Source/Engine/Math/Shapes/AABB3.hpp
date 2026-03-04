#pragma once
#include "Engine/Utility/Bounds.hpp"
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/VectorMath.hpp"
#include <limits>
#include <span>
#include <array>

namespace CLX
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
	using AABB3d = AABB3<double>;
    using AABB3i32 = AABB3<int32_t>;
	using AABB3i = AABB3i32;

	template<std::ranges::range R>
	[[nodiscard]] constexpr auto CreateAABB3FromPoints(const R& points)
	{
		using T = typename std::remove_cvref_t<decltype(points[0])>::value_type;

		Point3<T> min(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
		Point3<T> max(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest());

		for (const auto& p : points)
		{
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