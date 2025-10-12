#pragma once
#include "Utility/Shapes/AABB2.hpp"
#include "Utility/Shapes/AABB3.hpp"
#include "Utility/Shapes/LineSegment2.hpp"
#include "Utility/Shapes/LineSegment3.hpp"
#include "Utility/Shapes/Line2.hpp"
#include "Utility/Shapes/Line3.hpp"
#include "Utility/Shapes/Ray3.hpp"
#include "Utility/Shapes/Ray2.hpp"
#include "Utility/Shapes/Plane.hpp"
#include "Utility/Shapes/Triangle2.hpp"
#include "Utility/Shapes/Triangle3.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Math/VectorMath.hpp"
#include "Utility/Percent.hpp"

namespace Simple
{

	template<typename T>
	[[nodiscard]] constexpr Line2<T> ToLine(const LineSegment2<T>& lineSegment)
	{
		return Line2<T>(lineSegment.StartPoint(), lineSegment.EndPoint());
	}

	template<typename T>
	[[nodiscard]] constexpr Line3<T> ToLine(const LineSegment3<T>& lineSegment)
	{
		return Line3<T>(lineSegment.StartPoint(), lineSegment.EndPoint());
	}

	template<typename T>
	[[nodiscard]] constexpr LineSegment2<T> ToLineSegment(const Ray2<T>& ray, const T& distance)
	{
		return LineSegment2<T>(ray.GetOrigin(), ray.GetPointAtDistance(distance));
	}

	template<typename T>
	[[nodiscard]] constexpr LineSegment3<T> ToLineSegment(const Ray3<T>& ray, const T& distance)
	{
		return LineSegment3<T>(ray.GetOrigin(), ray.GetPointAtDistance(distance));
	}

	template<typename T>
	[[nodiscard]] constexpr Plane<T> ToPlane(const Triangle3<T>& triangle)
	{
		return Plane<T>(triangle.GetPoint0(), triangle.GetPoint1(), triangle.GetPoint2());
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> Remap(const Point2<T>& point, const AABB2<T>& from, const AABB2<T>& to)
	{
		return Remap(point, from.GetMin(), from.GetMax(), to.GetMin(), to.GetMax());
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> Remap0To1(const Point2<T>& point, const AABB2<T>& from)
	{
		return Remap(point, from.GetMin(), from.GetMax(), Point2<T>::Zero(), Point2<T>::One());
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> Remap(const Point3<T>& point, const AABB3<T>& from, const AABB3<T>& to)
	{
		return Remap(point, from.GetMin(), from.GetMax(), to.GetMin(), to.GetMax());
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> OffsetAABB(const AABB2<T>& aabb, const Vector2<T>& offset)
	{
		return AABB2<T>::CreateFromMinAndMax(aabb.GetMin() + offset, aabb.GetMax() + offset);
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> ToAABB2XY(const AABB3<T>& aabb)
	{
		return AABB2<T>::CreateFromMinAndMax(ToPoint2XY(aabb.GetMin()), ToPoint2XY(aabb.GetMax()));
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> ToAABB2XZ(const AABB3<T>& aabb)
	{
		return AABB2<T>::CreateFromMinAndMax(ToPoint2XZ(aabb.GetMin()), ToPoint2XZ(aabb.GetMax()));
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> ToAABB2YZ(const AABB3<T>& aabb)
	{
		return AABB2<T>::CreateFromMinAndMax(ToPoint2YZ(aabb.GetMin()), ToPoint2YZ(aabb.GetMax()));
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> ToAABB2(const LineSegment2<T>& lineSegment)
	{
		return CreateAABB2FromPoints(std::span<const Point2f>({ lineSegment.StartPoint(), lineSegment.EndPoint() }));
	}

	template<typename T>
	[[nodiscard]] constexpr T GetPlaneXPos(const Plane<T>& plane, const T& yPos, const T& zPos)
	{
		const Point3<T>& point = plane.GetPoint();
		const UnitVector3<T>& normal = plane.GetNormal();
		if (normal.X() == 0.f)
		{
			return std::numeric_limits<T>::infinity();
		}
		return point.x + (-(normal.Y() * (yPos - point.y) + normal.Z() * (zPos - point.z)) / normal.X());
	}

	template<typename T>
	[[nodiscard]] constexpr T GetPlaneYPos(const Plane<T>& plane, const T& xPos, const T& zPos)
	{
		const Point3<T>& point = plane.GetPoint();
		const UnitVector3<T>& normal = plane.GetNormal();
		if (normal.Y() == 0.f)
		{
			return std::numeric_limits<T>::infinity();
		}
		return point.y + (-(normal.X() * (xPos - point.x) + normal.Z() * (zPos - point.z)) / normal.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr T GetPlaneZPos(const Plane<T>& aPlane, const T& xPos, const T& yPos)
	{
		const Point3<T>& point = aPlane.GetPoint();
		const UnitVector3<T>& normal = aPlane.GetNormal();
		if (normal.Z() == 0.f)
		{
			return std::numeric_limits<T>::infinity();
		}
		return point.z + (-(normal.X() * (xPos - point.x) + normal.Y() * (yPos - point.y)) / normal.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsOnNormalSide(const Point3<T>& position, const Point3<T>& planePoint, const UnitVector3<T>& normal) noexcept
	{
		return Dot(position - planePoint, normal) > 0.0f;
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsNotOnNormalSide(const Point3<T>& position, const Point3<T>& planePoint, const UnitVector3<T>& normal) noexcept
	{
		return !IsOnNormalSide(position, planePoint, normal);
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsOnNormalSide(const Point3<T>& position, const Plane<T>& plane) noexcept
	{
		return IsOnNormalSide(position, plane.GetPoint(), plane.GetNormal());
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsNotOnNormalSide(const Point3<T>& position, const Plane<T>& plane) noexcept
	{
		return !IsOnNormalSide(position, plane);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetNormal(const Triangle3<T>& triangle)
	{
		const auto& p0 = triangle.GetPoint0();
		const auto& p1 = triangle.GetPoint1();
		const auto& p2 = triangle.GetPoint2();
		return UnitVector3<T>(Cross(p1 - p0, p2 - p0));
	}

	template<typename T>
	[[nodiscard]] constexpr std::array<UnitVector3<T>, 3> GetEdgeNormals(const Triangle3<T>& triangle)
	{
		const Point3<T>& p0 = triangle.GetPoint0();
		const Point3<T>& p1 = triangle.GetPoint1();
		const Point3<T>& p2 = triangle.GetPoint2();
		const UnitVector3<T> normal = GetNormal(triangle);
		return
		{
			GetPerpendicularVector(UnitVector3<T>(p1 - p0), normal),
			GetPerpendicularVector(UnitVector3<T>(p2 - p1), normal),
			GetPerpendicularVector(UnitVector3<T>(p0 - p2), normal),
		};
	}

	template<typename T>
	[[nodiscard]] constexpr T GetLength(const LineSegment2<T>& lineSegment)
	{
		return Distance(lineSegment.StartPoint(), lineSegment.EndPoint());
	}

	template<typename T>
	[[nodiscard]] constexpr T GetLength(const LineSegment3<T>& lineSegment)
	{
		return Distance(lineSegment.StartPoint(), lineSegment.EndPoint());
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> Lerp(const LineSegment2<T>& lineSegment, const float t)
	{
		return Lerp(lineSegment.StartPoint(), lineSegment.EndPoint(), t);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> Lerp(const LineSegment3<T>& lineSegment, const float t) noexcept
	{
		return Lerp(lineSegment.StartPoint(), lineSegment.EndPoint(), t);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> GetDirection(const LineSegment2<T>& lineSegment) noexcept
	{
		return GetUnitVector(lineSegment.StartPoint(), lineSegment.EndPoint());
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetDirection(const LineSegment3<T>& lineSegment) noexcept
	{
		return GetUnitVector(lineSegment.StartPoint(), lineSegment.EndPoint());
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> GetNormal(const LineSegment2<T>& lineSegment) noexcept
	{
		return GetPerpendicularVector(GetDirection(lineSegment));
	}

	template<typename T>
	[[nodiscard]] constexpr AABB2<T> GetBounds(const LineSegment2<T>& lineSegment)
	{
		return CreateAABB2FromPoints(std::array{ lineSegment.StartPoint(), lineSegment.EndPoint() });
	}

	template<typename T>
	[[nodiscard]] constexpr AABB3<T> GetBounds(const LineSegment3<T>& lineSegment)
	{
		auto points = std::array<Point3<T>, 2>{ lineSegment.StartPoint(), lineSegment.EndPoint() };
		return CreateAABB3FromPoints(points);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> GetProjectedPoint(const Plane<T>& plane, const Point3<T>& point) noexcept
	{
		const T t = Dot(plane.GetNormal(), point - plane.GetPoint());
		return point - plane.GetNormal() * t;
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> GetProjectedPoint(const Line2<T>& line, const Point2<T>& point) noexcept
	{
		const UnitVector2<T> normal = GetPerpendicularVector(line.GetDirection());
		const T t = Dot(normal, point - line.GetPoint());
		return point - normal * t;
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> GetProjectedPoint(const Line3<T>& line, const Point3<T>& point) noexcept
	{
		const T t = Dot(line.GetDirection(), point - line.GetPoint());
		return line.GetPoint() + line.GetDirection() * t;
	}

	template<typename T>
	[[nodiscard]] constexpr std::optional<Line3<T>> GetIntersectionLine(const Plane<T>& a, const Plane<T>& b)
	{
		const UnitVector3<T>& n1 = a.GetNormal();
		const UnitVector3<T>& n2 = b.GetNormal();
		const Vector3<T> direction = Cross<Vector3>(n1, n2);

		const T dirLengthSq = LengthSquared(direction);
		if (dirLengthSq < std::numeric_limits<T>::epsilon()) // parallel or same
		{
			return std::nullopt;
		}

		// Plane equations: n * x = d
		const T d1 = Dot(n1.X(), n1.Y(), n1.Z(), a.GetPoint().x, a.GetPoint().y, a.GetPoint().z);
		const T d2 = Dot(n2.X(), n2.Y(), n2.Z(), b.GetPoint().x, b.GetPoint().y, b.GetPoint().z);

		// Solve system: find point on line
		const Vector3<T> n1xn2 = direction;

		Vector3<T> pointOnLine =
			((Cross(n1xn2, n2) * d1) + (Cross(n1, n1xn2) * d2)) / dirLengthSq;

		return Line3<T>(Point3<T>::Zero() + pointOnLine, UnitVector3<T>(direction));
	}

	template<typename T>
	[[nodiscard]] constexpr T GetArea(const AABB2<T>& aabb) noexcept
	{
		const Vector2<T> size = aabb.GetExtent();
		return size.x * size.y;
	}

	template<typename T>
	[[nodiscard]] constexpr T GetVolume(const AABB3<T>& aabb) noexcept
	{
		const Vector3<T> size = aabb.GetExtent();
		return size.x * size.y * size.z;
	}

	template<typename T>
	[[nodiscard]] constexpr T Measure(const AABB2<T>& aabb) noexcept
	{
		return GetArea(aabb);
	}

	template<typename T>
	[[nodiscard]] constexpr T Measure(const AABB3<T>& aabb) noexcept
	{
		return GetVolume(aabb);
	}

	template<typename T>
	[[nodiscard]] constexpr std::optional<AABB2<T>> GetOverlap(const AABB2<T>& a, const AABB2<T>& b) noexcept
	{
		if (a.GetMax().x < b.GetMin().x || a.GetMin().x > b.GetMax().x ||
			a.GetMax().y < b.GetMin().y || a.GetMin().y > b.GetMax().y)
		{
			return std::nullopt; // No overlap
		}

		return AABB2<T>::CreateFromMinAndMax(
			Max(a.GetMin(), b.GetMin()),
			Min(a.GetMax(), b.GetMax())
		);
	}

	template<typename T>
	[[nodiscard]] constexpr std::optional<AABB3<T>> GetOverlap(const AABB3<T>& a, const AABB3<T>& b) noexcept
	{
		if (a.GetMax().x < b.GetMin().x || a.GetMin().x > b.GetMax().x ||
			a.GetMax().y < b.GetMin().y || a.GetMin().y > b.GetMax().y ||
			a.GetMax().z < b.GetMin().z || a.GetMin().z > b.GetMax().z)
		{
			return std::nullopt; // No overlap
		}
		return AABB3<T>::CreateFromMinAndMax(
			Max(a.GetMin(), b.GetMin()),
			Min(a.GetMax(), b.GetMax())
		);
	}

	// How much of the area of AABB a is covered by the overlap with AABB b
	template<typename Ret = float, typename T>
	[[nodiscard]] constexpr Percent<Ret> GetOverlapPercentage(const AABB2<T>& a, const AABB2<T>& b) noexcept
	{
		const auto overlap = GetOverlap(a, b);
		if (!overlap.has_value())
		{
			return Percent<Ret>(0.0f); // No overlap
		}
		return Percent<Ret>(static_cast<Ret>(Measure(overlap.value())) / static_cast<Ret>(Measure(a)));
	}

	// How much of the area of AABB a is covered by the overlap with AABB b
	template<typename Ret = float, typename T>
	[[nodiscard]] constexpr Percent<Ret> GetOverlapPercentage(const AABB3<T>& a, const AABB3<T>& b) noexcept
	{
		const auto overlap = GetOverlap(a, b);
		if (!overlap.has_value())
		{
			return Percent<Ret>(0.0f); // No overlap
		}
		return Percent<Ret>(static_cast<Ret>(Measure(overlap.value())) / static_cast<Ret>(Measure(a)));
	}

	template<typename Ret = float, typename T>
	[[nodiscard]] constexpr Ret GetArea(const Triangle2<T>& triangle) noexcept
	{
		const Point2<T>& p0 = triangle.GetPoint0();
		const Point2<T>& p1 = triangle.GetPoint1();
		const Point2<T>& p2 = triangle.GetPoint2();
		return static_cast<Ret>(0.5f * Abs(
			p0.x * (p1.y - p2.y) +
			p1.x * (p2.y - p0.y) +
			p2.x * (p0.y - p1.y)
		));
	}
}