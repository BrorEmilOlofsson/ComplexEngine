#pragma once
#include "Utility/Bounds.hpp"
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/VectorMath.hpp"
#include <iostream>
#include <span>
#include <limits>

namespace Simple
{
	struct AABB2BoundsChecker final
	{
		template<typename T>
		[[nodiscard]] constexpr bool operator()(const Point2<T>& min, const Point2<T>& max) const
		{
			return min.x <= max.x && min.y <= max.y;
		}
	};

	template<typename T>
	using AABB2 = Bounds<Point2<T>, AABB2BoundsChecker>;

	using AABB2f = AABB2<float>;
	using AABB2i = AABB2<int>;
	using AABB2ui = AABB2<unsigned int>;
	using AABB2l = AABB2<long>;

	template<typename T, typename U>
	[[nodiscard]] constexpr AABB2<T> ToAABB(const AABB2<U>& aabb)
	{
		return AABB2<T>::CreateFromMinAndMax(Point2<T>(aabb.GetMin()), Point2<T>(aabb.GetMax()));
	}

	template<typename T>
	constexpr AABB2<T> CreateAABB2FromPoints(std::span<const Point2<T>> points)
	{
		Point2<T> min(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
		Point2<T> max(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest());

		for (const auto& p : points)
		{
			min = Min(min, p);
			max = Max(max, p);
		}

		return AABB2<T>::CreateFromMinAndMax(min, max);
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> ScaleAABB(const AABB2<T>& aabb, Vector2f scale, Vector2f pivot = Vector2f(0.5f, 0.5f))
	{
		const Vector2<T> extent = static_cast<Vector2<T>>(static_cast<Vector2f>(aabb.GetExtent()) * scale);
		const Point2<T> min = aabb.GetMin();
		const Point2<T> max = aabb.GetMax();
		const Point2<T> center = LerpPoint(min, max, pivot);
		return AABB2<T>::CreateFromCenterAndExtent(center, extent);
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> ScaleAABB(const AABB2<T>& aabb, float scale, Vector2f pivot = Vector2f(0.5f, 0.5f))
	{
		return ScaleAABB(aabb, Vector2f(scale, scale), pivot);
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInside(const Point2<T>& point, const Point2<T>& min, const Point2<T>& max) noexcept
	{
		return !(point.x < min.x || point.y < min.y
			|| point.x > max.x || point.y > max.y);
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInside(const Point2<T>& point, const AABB2<T>& aabb) noexcept
	{
		return IsInside(point, aabb.GetMin(), aabb.GetMax());
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> GetTopLeftCorner(const AABB2<T>& aabb) noexcept
	{
		return Point2<T>(aabb.GetMin().x, aabb.GetMax().y);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> GetBottomRightCorner(const AABB2<T>& aabb) noexcept
	{
		return Point2<T>(aabb.GetMin().x, aabb.GetMax().y);
	}

	template<typename T, typename ContainerT = std::array<Point2<T>, 4>>
	[[nodiscard]] constexpr ContainerT GetCorners(const AABB2<T>& aabb)
	{
		const Point3<T> min = aabb.GetMin();
		const Point3<T> max = aabb.GetMax();
		return
		{
			min,
			Point2<T>(max.x, min.y),
			max,
			Point2<T>(min.x, max.y)
		};
	}


	template<typename T>
	std::ostream& operator<<(std::ostream& os, const AABB2<T>& aabb)
	{
		os << "Min: " << aabb.GetMin() << ", Max: " << aabb.GetMax();
		return os;
	}
}