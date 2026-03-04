#pragma once
#include <limits>
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Math/Shapes/Plane.hpp"
#include "Engine/Math/Shapes/Sphere.hpp"
#include "Engine/Math/Shapes/Ray3.hpp"
#include "Engine/Math/Shapes/Disk.hpp"
#include "Engine/Math/Shapes/Triangle3.hpp"
#include "Engine/Math/Shapes/Line2.hpp"
#include "Engine/Math/Shapes/Line3.hpp"
#include "Engine/Math/Shapes/LineSegment2.hpp"
#include "Engine/Math/Math.hpp"
#include "Engine/Math/VectorMath.hpp"
#include "Engine/Math/ShapeMath.hpp"

namespace Simple
{

	struct RayHitResult
	{
		Point3f hitPoint;
		float distance = 0.f;
	};;

	[[nodiscard]] constexpr std::optional<RayHitResult> IntersectionRaySphere(const Ray3f& ray, const Spheref& sphere)
	{
		const float b = 2.0f * Dot(ray.GetOrigin() - sphere.GetCenter(), ray.GetDirection());
		const float c = DistanceSquared(ray.GetOrigin(), sphere.GetCenter()) - Square(sphere.GetRadius().Value());
		const float discriminant = b * b - 4.0f * c;

		if (discriminant < 0.0f)
		{
			return std::nullopt; // No intersection
		}

		const float sqrtDiscriminant = Sqrt(discriminant);
		const float t0 = (-b - sqrtDiscriminant) * 0.5f;
		const float t1 = (-b + sqrtDiscriminant) * 0.5f;

		const float t = (t0 >= 0.0f) ? t0 : (t1 >= 0.0f ? t1 : -1.0f);
		if (t < 0.0f)
		{
			return std::nullopt; // Intersection is behind the ray
		}

		return std::optional(RayHitResult(ray.GetPointAtDistance(t), t));
	}

	[[nodiscard]] constexpr std::optional<RayHitResult> IntersectionRayPlane(const Ray3f& ray, const Planef& plane)
	{
		const float dotProduct = Dot(plane.GetNormal(), ray.GetDirection());

		if (dotProduct == 0.0f)
		{
			return std::nullopt;
		}

		const Vector3f g = plane.GetPoint() - ray.GetOrigin();
		const float t = Dot(g, plane.GetNormal()) / dotProduct;

		if (t < 0)
		{
			return std::nullopt;
		}

		return std::optional(RayHitResult(ray.GetPointAtDistance(t), t));
	}

	[[nodiscard]] constexpr std::optional<RayHitResult> IntersectionRayAABB(const Ray3f& ray, const AABB3f& aabb)
	{
		const Point3f& rayOrigin = ray.GetOrigin();
		const UnitVector3f& rayDirection = ray.GetDirection();

		const Point3f& min = aabb.GetMin();
		const Point3f& max = aabb.GetMax();

		if (Abs(rayDirection.X()) < std::numeric_limits<float>::epsilon())
		{
			if (rayOrigin.x < min.x || rayOrigin.x > max.x)
			{
				return std::nullopt;
			}
		}

		float txMin = (min.x - rayOrigin.x) / rayDirection.X();
		float txMax = (max.x - rayOrigin.x) / rayDirection.X();

		if (txMin > txMax)
		{
			std::swap(txMin, txMax);
		}

		if (Abs(rayDirection.Y()) < std::numeric_limits<float>::epsilon())
		{
			if (rayOrigin.y < min.y || rayOrigin.y > max.y)
			{
				return std::nullopt;
			}
		}

		float tyMin = (min.y - rayOrigin.y) / rayDirection.Y();
		float tyMax = (max.y - rayOrigin.y) / rayDirection.Y();

		if (tyMin > tyMax)
		{
			std::swap(tyMin, tyMax);
		}

		if ((txMin > tyMax) || (tyMin > txMax))
		{
			return std::nullopt;
		}

		if (tyMin > txMin)
		{
			txMin = tyMin;
		}

		if (tyMax < txMax)
		{
			txMax = tyMax;
		}

		if (Abs(rayDirection.Z()) < std::numeric_limits<float>::epsilon())
		{
			if (rayOrigin.z < min.z || rayOrigin.z > max.z)
			{
				return std::nullopt;
			}
		}

		float tzMin = (min.z - rayOrigin.z) / rayDirection.Z();
		float tzMax = (max.z - rayOrigin.z) / rayDirection.Z();

		if (tzMin > tzMax)
		{
			std::swap(tzMin, tzMax);
		}

		if ((txMin > tzMax) || (tzMin > txMax))
		{
			return std::nullopt;
		}

		const float tMin = Max(Max(txMin, tyMin), tzMin);
		if (tMin < 0.0f)
		{
			return std::nullopt;
		}

		return std::optional(RayHitResult(ray.GetPointAtDistance(tMin), tMin));
	}

	[[nodiscard]] constexpr std::optional<RayHitResult> IntersectionRayDisk(const Ray3f& ray, const Diskf& disk)
	{
		const auto planeResult = IntersectionRayPlane(ray, Planef::FromPointAndNormal(disk.GetCenter(), disk.GetNormal()));

		if (planeResult)
		{
			if (IsInRange(planeResult->hitPoint, disk.GetCenter(), disk.GetRadius().Value()))
			{
				return planeResult;
			}
		}

		return std::nullopt;
	}


	[[nodiscard]] constexpr std::optional<RayHitResult> IntersectionRayTriangle(const Ray3f& ray, const Triangle3f& triangle)
	{
		const auto rayHitResult = IntersectionRayPlane(ray, ToPlane(triangle));

		if (!rayHitResult)
		{
			return std::nullopt;
		}

		const Point3f& hitPoint = rayHitResult->hitPoint;
		const auto edgeNormals = GetEdgeNormals(triangle);

		if (IsOnNormalSide(hitPoint, triangle.GetPoint0(), edgeNormals[0])
			|| IsOnNormalSide(hitPoint, triangle.GetPoint1(), edgeNormals[1])
			|| IsOnNormalSide(hitPoint, triangle.GetPoint2(), edgeNormals[2]))
		{
			return std::nullopt;
		}

		return rayHitResult;
	}

	[[nodiscard]] constexpr std::optional<Point3f> IntersectionLinePlane(const Line3f& line, const Planef& plane)
	{
		const float dotProduct = Dot(plane.GetNormal(), line.GetDirection());

		if (dotProduct == 0.0f)
		{
			return std::nullopt;
		}

		const Vector3f g = plane.GetPoint() - line.GetPoint();
		const float t = Dot(g, plane.GetNormal()) / dotProduct;

		return std::optional(line.GetPoint() + line.GetDirection() * t);
	}

	[[nodiscard]] constexpr std::optional<Point2f> IntersectionLine2DVsLine2D(const Line2f& line1, const Line2f& line2)
	{
		const Point2f& l1P = line1.GetPoint();
		const UnitVector2f& l1D = line1.GetDirection();
		const Point2f& l2P = line2.GetPoint();
		const UnitVector2f& l2D = line2.GetDirection();

		const float denominator = l2D.Y() * l1D.X() - l1D.Y() * l2D.X();

		if (denominator == 0.0f)
		{
			return std::nullopt;
		}

		const float t = (l2D.Y() * (l2P.x - l1P.x) - l2D.X() * (l2P.y - l1P.y)) / denominator;
		const Point2f intersectionPoint = l1P + l1D * t;
		return intersectionPoint;
	}

	[[nodiscard]] constexpr std::optional<Point2f> IntersectionLineSegment2DVsLineSegment2D(const LineSegment2f& lineSegment1, const LineSegment2f& lineSegment2)
	{
		const std::optional<Point2f> intersectionLine = IntersectionLine2DVsLine2D(ToLine(lineSegment1), ToLine(lineSegment2));
		if (intersectionLine)
		{
			const float distance1 = Distance(lineSegment1.StartPoint(), intersectionLine.value());
			const float distance2 = Distance(lineSegment1.EndPoint(), intersectionLine.value());
			const float line1Length = GetLength(lineSegment1);
			if (distance1 <= line1Length && distance2 <= line1Length)
			{
				return intersectionLine;
			}
		}
		return std::nullopt;
	}


}