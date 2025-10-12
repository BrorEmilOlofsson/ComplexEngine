#pragma once
#include "Utility/Shapes/Ray3.hpp"
#include "Utility/Shapes/AABB3.hpp"
#include "Utility/Shapes/Sphere.hpp"
#include "Utility/Shapes/Cylinder.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Math/VectorMath.hpp"

#undef min

namespace Simple
{

	template<typename T>
	constexpr bool DetectCollision(const Point3<T>& aPoint1, const Point3<T>& aPoint2)
	{
		return aPoint1 == aPoint2;
	}

	template<typename T>
	constexpr bool DetectCollision(const Point3<T>& aPoint, const Sphere<T>& aSphere)
	{
		const T distanceSquared = DistanceSquared(aPoint, aSphere.GetCenter());
		const T sumRadiiSquared = Square(aSphere.GetRadius());
		return distanceSquared <= sumRadiiSquared;
	}

	template<typename T>
	constexpr bool DetectCollision(const Sphere<T>& aSphere1, const Sphere<T>& aSphere2)
	{
		const T distanceSquared = DistanceSquared(aSphere1.GetCenter(), aSphere2.GetCenter());
		const T sumRadiiSquared = Square(aSphere1.GetRadius() + aSphere2.GetRadius());
		return distanceSquared <= sumRadiiSquared;
	}

	constexpr bool DetectCollision(const Spheref& aSphere, const AABB3f& aAABB)
	{
		const Point3f sphereCenter = aSphere.GetCenter();
		// Find the closest point to the sphere within the AABB
		Point3f closestPoint
		{
			Max(aAABB.GetMin().x, Min(sphereCenter.x, aAABB.GetMax().x)),
			Max(aAABB.GetMin().y, Min(sphereCenter.y, aAABB.GetMax().y)),
			Max(aAABB.GetMin().z, Min(sphereCenter.z, aAABB.GetMax().z))
		};

		// Calculate the distance between the closest point and the sphere's center
		const float distanceSquared = (closestPoint.x - sphereCenter.x) * (closestPoint.x - sphereCenter.x) +
			(closestPoint.y - sphereCenter.y) * (closestPoint.y - sphereCenter.y) +
			(closestPoint.z - sphereCenter.z) * (closestPoint.z - sphereCenter.z);

		// If the distance is less than the radius, they intersect
		return distanceSquared < (aSphere.GetRadius() * aSphere.GetRadius());
	}

	constexpr bool DetectCollision(const Spheref& aSphere, const Ray3f& aRay)
	{
		const Vector3f rayOriginToSphere = aSphere.GetCenter() - aRay.GetOrigin();

		const float sphereRadiusSqr = aSphere.GetRadius() * aSphere.GetRadius();
		if (LengthSquared(rayOriginToSphere) <= sphereRadiusSqr)
		{
			return true;
		}

		const float projection = Dot(rayOriginToSphere, aRay.GetDirection());

		if (projection < 0)
		{
			return false;
		}

		const Point3f closestPoint = aRay.GetOrigin() + projection * aRay.GetDirection();

		const float distanceSqr = DistanceSquared(closestPoint, aSphere.GetCenter());

		return distanceSqr <= sphereRadiusSqr;
	}

	constexpr bool DetectCollision(const AABB3f& aAABB1, const AABB3f& aAABB2)
	{
		// Check for overlap along the x-axis
		if (aAABB1.GetMax().x < aAABB2.GetMin().x || aAABB1.GetMin().x > aAABB2.GetMax().x)
		{
			return false; // No overlap
		}

		// Check for overlap along the y-axis
		if (aAABB1.GetMax().y < aAABB2.GetMin().y || aAABB1.GetMin().y > aAABB2.GetMax().y)
		{
			return false; // No overlap
		}

		// Check for overlap along the z-axis
		if (aAABB1.GetMax().z < aAABB2.GetMin().z || aAABB1.GetMin().z > aAABB2.GetMax().z)
		{
			return false; // No overlap
		}

		// If there is no gap along any axis, the AABBs overlap
		return true;
	}

	constexpr bool DetectCollision(const AABB3f& aAABB, const Spheref& aSphere)
	{
		return DetectCollision(aSphere, aAABB);
	}

	constexpr bool DetectCollision(const AABB3f& aAABB, const Ray3f& aRay)
	{
		const Vector3f invDir = 1.0f / aRay.GetDirection();

		float tMin = (aAABB.GetMin().x - aRay.GetOrigin().x) * invDir.x;
		float tMax = (aAABB.GetMax().x - aRay.GetOrigin().x) * invDir.x;

		if (tMin > tMax) std::swap(tMin, tMax);

		float tYMin = (aAABB.GetMin().y - aRay.GetOrigin().y) * invDir.y;
		float tYMax = (aAABB.GetMax().y - aRay.GetOrigin().y) * invDir.y;

		if (tYMin > tYMax) std::swap(tYMin, tYMax);

		if ((tMin > tYMax) || (tYMin > tMax)) return false;

		if (tYMin > tMin) tMin = tYMin;
		if (tYMax < tMax) tMax = tYMax;

		float tZMin = (aAABB.GetMin().z - aRay.GetOrigin().z) * invDir.z;
		float tZMax = (aAABB.GetMax().z - aRay.GetOrigin().z) * invDir.z;

		if (tZMin > tZMax) std::swap(tZMin, tZMax);

		if ((tMin > tZMax) || (tZMin > tMax)) return false;

		return true;
	}

	constexpr bool DetectCollision(const Ray3f& aRay, const Spheref& aSphere)
	{
		return DetectCollision(aSphere, aRay);
	}

	constexpr bool DetectCollision(const Ray3f& aRay, const AABB3f& aAABB)
	{
		return DetectCollision(aAABB, aRay);
	}

	constexpr bool DetectCollision(const Ray3f& aRay1, const Ray3f& aRay2)
	{
		constexpr float TOLERANCE = 1e-6f;

		const Point3f origin1 = aRay1.GetOrigin();
		const Point3f origin2 = aRay2.GetOrigin();
		const UnitVector3f dir1 = aRay1.GetDirection();
		const UnitVector3f dir2 = aRay2.GetDirection();

		const Vector3f r = origin2 - origin1;
		const float a = Dot(dir1, dir1);
		const float b = Dot(dir1, dir2);
		const float c = Dot(dir2, dir2);
		const float d = Dot(dir1, r);
		const float e = Dot(dir2, r);

		const float denominator = a * c - b * b;

		// Check for parallelism
		if (Abs(denominator) < TOLERANCE)
		{

			const Vector3f rayToPoint = aRay1.GetOrigin() - aRay2.GetOrigin();

			if (Abs(dir2.X() * rayToPoint.y - dir2.Y() * rayToPoint.x) < 1e-6 &&
				Abs(dir2.Y() * rayToPoint.z - dir2.Z() * rayToPoint.y) < 1e-6 &&
				Abs(dir2.Z() * rayToPoint.x - dir2.X() * rayToPoint.z) < 1e-6)
			{

				const float dotProduct = Dot(dir2, rayToPoint);

				if (dotProduct >= 0)
				{
					return true;
				}
			}

			return false;
		}

		const float t1 = (d * c - b * e) / denominator;
		const float t2 = (e * a - b * d) / denominator;

		const Point3f point1 = origin1 + t1 * dir1;
		const Point3f point2 = origin2 + t2 * dir2;

		return Distance(point1, point2) < TOLERANCE;

		//if (Distance(point1, point2) < TOLERANCE)
		//{
		//	return true; // Rays intersect at point1 (or point2)
		//}
		//else
		//{
		//	return false; // Rays do not intersect
		//}
	}


	template<typename T>
	constexpr bool DetectCollision(const Cylinder<T>& aCylinder1, const Cylinder<T>& aCylinder2)
	{
		aCylinder1;
		aCylinder2;
		return false;
	}

	template<typename T>
	constexpr bool DetectCollision(const Sphere<T>& aSphere, const Cylinder<T>& aCylinder)
	{
		const Point3<T>& sphereCenter = aSphere.GetCenter();
		const T sphereRadius = aSphere.GetRadius();
		const Point3<T> cylinderP0 = aCylinder.GetLowerPoint();
		const Point3<T> cylinderP1 = aCylinder.GetUpperPoint();
		const T cylinderRadius = aCylinder.GetRadius();

		const UnitVector3<T>& cylinderAxis = aCylinder.GetAxis();
		//const Vector3<T> cylinderAxis = cylinderP1 - cylinderP0;
		const T cylinderHeightSq = Square(aCylinder.GetHeight()); // Length squared
		if (cylinderHeightSq == T(0))
		{
			// Degenerate cylinder -> treat as sphere
			const T distSq = DistanceSquared(sphereCenter, cylinderP0);
			const T radiusSum = sphereRadius + cylinderRadius;
			return distSq <= radiusSum * radiusSum;
		}

		const Vector3<T> toSphere = sphereCenter - cylinderP0;

		// Project sphere center onto cylinder axis
		const T t = Dot(toSphere, cylinderAxis);
		//t = Clamp(t, T(0), T(1)); // Clamp to finite segment

		const Point3<T> closestPoint = cylinderP0 + t * cylinderAxis;

		// Compute distance from sphere center to the closest point
		const T distSq = DistanceSquared(sphereCenter, closestPoint);
		const T radiusSum = sphereRadius + cylinderRadius;
		return distSq <= radiusSum * radiusSum;
	}

	template<typename T>
	constexpr bool DetectCollision(const Cylinder<T>& aCylinder, const Sphere<T>& aSphere)
	{
		return DetectCollision(aSphere, aCylinder);
	}


}