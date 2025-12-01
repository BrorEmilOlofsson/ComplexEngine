#pragma once
#include "Utility/Shapes/Ray3.hpp"
#include "Utility/Shapes/AABB3.hpp"
#include "Utility/Shapes/Sphere.hpp"
#include "Utility/Shapes/Cylinder.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Math/VectorMath.hpp"
#include "Utility/Math/Point3.hpp"
#include <utility>

namespace Simple
{

	template<typename T>
	constexpr bool DetectCollision(const Point3<T>& point1, const Point3<T>& point2)
	{
		return point1 == point2;
	}

	template<typename T>
	constexpr bool DetectCollision(const Point3<T>& point, const Sphere<T>& sphere)
	{
		const T distanceSquared = DistanceSquared(point, sphere.GetCenter());
		const T sumRadiiSquared = Square(sphere.GetRadius());
		return distanceSquared <= sumRadiiSquared;
	}

	template<typename T>
	constexpr bool DetectCollision(const Sphere<T>& sphere1, const Sphere<T>& sphere2)
	{
		const T distanceSquared = DistanceSquared(sphere1.GetCenter(), sphere2.GetCenter());
		const T sumRadiiSquared = Square(sphere1.GetRadius() + sphere2.GetRadius());
		return distanceSquared <= sumRadiiSquared;
	}

	constexpr bool DetectCollision(const Spheref& sphere, const AABB3f& aabb)
	{
		const Point3f sphereCenter = sphere.GetCenter();
		// Find the closest point to the sphere within the AABB
		Point3f closestPoint
		{
			Max(aabb.GetMin().x, Min(sphereCenter.x, aabb.GetMax().x)),
			Max(aabb.GetMin().y, Min(sphereCenter.y, aabb.GetMax().y)),
			Max(aabb.GetMin().z, Min(sphereCenter.z, aabb.GetMax().z))
		};

		// Calculate the distance between the closest point and the sphere's center
		const float distanceSquared = (closestPoint.x - sphereCenter.x) * (closestPoint.x - sphereCenter.x) +
			(closestPoint.y - sphereCenter.y) * (closestPoint.y - sphereCenter.y) +
			(closestPoint.z - sphereCenter.z) * (closestPoint.z - sphereCenter.z);

		// If the distance is less than the radius, they intersect
		return distanceSquared < (sphere.GetRadius() * sphere.GetRadius());
	}

	constexpr bool DetectCollision(const Spheref& sphere, const Ray3f& ray)
	{
		const Vector3f rayOriginToSphere = sphere.GetCenter() - ray.GetOrigin();

		const float sphereRadiusSqr = sphere.GetRadius() * sphere.GetRadius();
		if (LengthSquared(rayOriginToSphere) <= sphereRadiusSqr)
		{
			return true;
		}

		const float projection = Dot(rayOriginToSphere, ray.GetDirection());

		if (projection < 0)
		{
			return false;
		}

		const Point3f closestPoint = ray.GetOrigin() + projection * ray.GetDirection();

		const float distanceSqr = DistanceSquared(closestPoint, sphere.GetCenter());

		return distanceSqr <= sphereRadiusSqr;
	}

	constexpr bool DetectCollision(const AABB3f& aabb1, const AABB3f& aabb2)
	{
		// Check for overlap along the x-axis
		if (aabb1.GetMax().x < aabb2.GetMin().x || aabb1.GetMin().x > aabb2.GetMax().x)
		{
			return false; // No overlap
		}

		// Check for overlap along the y-axis
		if (aabb1.GetMax().y < aabb2.GetMin().y || aabb1.GetMin().y > aabb2.GetMax().y)
		{
			return false; // No overlap
		}

		// Check for overlap along the z-axis
		if (aabb1.GetMax().z < aabb2.GetMin().z || aabb1.GetMin().z > aabb2.GetMax().z)
		{
			return false; // No overlap
		}

		// If there is no gap along any axis, the AABBs overlap
		return true;
	}

	constexpr bool DetectCollision(const AABB3f& aabb, const Spheref& sphere)
	{
		return DetectCollision(sphere, aabb);
	}

	constexpr bool DetectCollision(const AABB3f& aabb, const Ray3f& ray)
	{
		const UnitVector3f rayDir = ray.GetDirection();
		const Point3f rayOrigin = ray.GetOrigin();
		const Point3f aabbMin = aabb.GetMin();
		const Point3f aabbMax = aabb.GetMax();

		const float invX = 1.f / rayDir.X();
		float tMin = (aabbMin.x - rayOrigin.x) * invX;
		float tMax = (aabbMax.x - rayOrigin.x) * invX;

		if (tMin > tMax) std::swap(tMin, tMax);

		const float invY = 1.f / rayDir.Y();
		float tYMin = (aabbMin.y - rayOrigin.y) * invY;
		float tYMax = (aabbMax.y - rayOrigin.y) * invY;

		if (tYMin > tYMax) std::swap(tYMin, tYMax);

		if ((tMin > tYMax) || (tYMin > tMax)) return false;

		if (tYMin > tMin) tMin = tYMin;
		if (tYMax < tMax) tMax = tYMax;

		const float invZ = 1.f / rayDir.Z();
		float tZMin = (aabbMin.z - rayOrigin.z) * invZ;
		float tZMax = (aabbMax.z - rayOrigin.z) * invZ;

		if (tZMin > tZMax) std::swap(tZMin, tZMax);

		if ((tMin > tZMax) || (tZMin > tMax)) return false;

		return true;
	}

	constexpr bool DetectCollision(const Ray3f& ray, const Spheref& sphere)
	{
		return DetectCollision(sphere, ray);
	}

	constexpr bool DetectCollision(const Ray3f& ray, const AABB3f& aabb)
	{
		return DetectCollision(aabb, ray);
	}

	constexpr bool DetectCollision(const Ray3f& ray1, const Ray3f& ray2)
	{
		constexpr float TOLERANCE = 1e-6f;

		const Point3f origin1 = ray1.GetOrigin();
		const Point3f origin2 = ray2.GetOrigin();
		const UnitVector3f dir1 = ray1.GetDirection();
		const UnitVector3f dir2 = ray2.GetDirection();

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

			const Vector3f rayToPoint = ray1.GetOrigin() - ray2.GetOrigin();

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
	constexpr bool DetectCollision(const Cylinder<T>& cylinder1, const Cylinder<T>& cylinder2)
	{
		cylinder1;
		cylinder2;
		throw std::exception("DetectCollision for Cylinder-Cylinder not implemented yet.");
		//return false;
	}

	template<typename T>
	constexpr bool DetectCollision(const Sphere<T>& sphere, const Cylinder<T>& cylinder)
	{
		const Point3<T>& sphereCenter = sphere.GetCenter();
		const T sphereRadius = sphere.GetRadius();
		const Point3<T> cylinderP0 = cylinder.GetLowerPoint();
		const Point3<T> cylinderP1 = cylinder.GetUpperPoint();
		const T cylinderRadius = cylinder.GetRadius();

		const UnitVector3<T>& cylinderAxis = cylinder.GetAxis();
		//const Vector3<T> cylinderAxis = cylinderP1 - cylinderP0;
		const T cylinderHeightSq = Square(cylinder.GetHeight()); // Length squared
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
	constexpr bool DetectCollision(const Cylinder<T>& cylinder, const Sphere<T>& sphere)
	{
		return DetectCollision(sphere, cylinder);
	}


}