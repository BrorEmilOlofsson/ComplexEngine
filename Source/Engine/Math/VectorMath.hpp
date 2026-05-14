#pragma once
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Math.hpp"
#include "Point3.hpp"
#include "Point2.hpp"
#include "UnitVector2.hpp"
#include "UnitVector3.hpp"
#include "Dimension2.hpp"

namespace CLX
{

	template<typename T>
	[[nodiscard]] constexpr const T& GetX(const Point3<T>& point) noexcept
	{
		return point.x;
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetY(const Point3<T>& point) noexcept
	{
		return point.y;
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetZ(const Point3<T>& point) noexcept
	{
		return point.z;
	}

	template<typename T>
	[[nodiscard]] constexpr T& GetX(Point3<T>& point) noexcept
	{
		return point.x;
	}

	template<typename T>
	[[nodiscard]] constexpr T& GetY(Point3<T>& point) noexcept
	{
		return point.y;
	}

	template<typename T>
	[[nodiscard]] constexpr T& GetZ(Point3<T>& point) noexcept
	{
		return point.z;
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetX(const Vector3<T>& vector) noexcept
	{
		return vector.x;
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetY(const Vector3<T>& vector) noexcept
	{
		return vector.y;
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetZ(const Vector3<T>& vector) noexcept
	{
		return vector.z;
	}

	template<typename T>
	[[nodiscard]] constexpr T& GetX(Vector3<T>& vector) noexcept
	{
		return vector.x;
	}

	template<typename T>
	[[nodiscard]] constexpr T& GetY(Vector3<T>& vector) noexcept
	{
		return vector.y;
	}

	template<typename T>
	[[nodiscard]] constexpr T& GetZ(Vector3<T>& vector) noexcept
	{
		return vector.z;
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetX(const UnitVector3<T>& vector) noexcept
	{
		return vector.X();
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetY(const UnitVector3<T>& vector) noexcept
	{
		return vector.Y();
	}

	template<typename T>
	[[nodiscard]] constexpr const T& GetZ(const UnitVector3<T>& vector) noexcept
	{
		return vector.Z();
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> ToVector4(const Vector3<T>& vector, const T& w = 1) noexcept
	{
		return Vector4<T>(vector.x, vector.y, vector.z, w);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> ToVector4(const Point3<T>& point, const T& w = 1) noexcept
	{
		return Vector4<T>(point.x, point.y, point.z, w);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> ToVector3(const Vector4<T>& vector) noexcept
	{
		return Vector3<T>(vector.x, vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> ToVector3(const UnitVector3<T>& vector) noexcept
	{
		return Vector3<T>(vector.X(), vector.Y(), vector.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> ToVector3XY(const Vector2<T>& vector, const T z = 0.0) noexcept
	{
		return Vector3<T>(vector.x, vector.y, z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> ToVector3XZ(const Vector2<T>& vector, const T y = 0.0) noexcept
	{
		return Vector3<T>(vector.x, y, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> ToVector3YZ(const Vector2<T>& vector, const T x = 0.0) noexcept
	{
		return Vector3<T>(x, vector.x, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> ToVector2XY(const Vector3<T>& vector) noexcept
	{
		return Vector2<T>(vector.x, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> ToVector2XZ(const Vector3<T>& vector) noexcept
	{
		return Vector2<T>(vector.x, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> ToVector2YZ(const Vector3<T>& vector) noexcept
	{
		return Vector2<T>(vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> ToPoint3(const Vector4<T>& vector) noexcept
	{
		return Point3<T>(vector.x, vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> ToPoint2XY(const Point3<T>& point) noexcept
	{
		return Point2<T>(point.x, point.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> ToPoint2XZ(const Point3<T>& point) noexcept
	{
		return Point2<T>(point.x, point.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> ToPoint2YZ(const Point3<T>& point) noexcept
	{
		return Point2<T>(point.y, point.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> ToPoint3XY(const Point2<T>& point, const T& z = T{}) noexcept
	{
		return Point3<T>(point.x, point.y, z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> ToPoint3XZ(const Point2<T>& point, const T& y = T{}) noexcept
	{
		return Point3<T>(point.x, y, point.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> ToPoint3YZ(const Point2<T>& point, const T& x = T{}) noexcept
	{
		return Point3<T>(x, point.x, point.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> RemoveX(const Vector2<T>& vector) noexcept
	{
		return Vector2<T>(0, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> RemoveY(const Vector2<T>& vector) noexcept
	{
		return Vector2<T>(vector.x, 0);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> RemoveX(const Point2<T>& point) noexcept
	{
		return Point2<T>(0, point.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> RemoveY(const Point2<T>& point) noexcept
	{
		return Point2<T>(point.x, 0);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> RemoveX(const UnitVector2<T>& vector)
	{
		return UnitVector2<T>(0, vector.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> RemoveY(const UnitVector2<T>& vector)
	{
		return UnitVector2<T>(vector.X(), 0);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> RemoveX(const Vector3<T>& vector) noexcept
	{
		return Vector3<T>(0, vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> RemoveY(const Vector3<T>& vector) noexcept
	{
		return Vector3<T>(vector.x, 0, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> RemoveZ(const Vector3<T>& vector) noexcept
	{
		return Vector3<T>(vector.x, vector.y, 0);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> RemoveX(const Point3<T>& point) noexcept
	{
		return Point3<T>(0, point.y, point.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> RemoveY(const Point3<T>& point) noexcept
	{
		return Point3<T>(point.x, 0, point.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> RemoveZ(const Point3<T>& point) noexcept
	{
		return Point3<T>(point.x, point.y, 0);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> RemoveX(const UnitVector3<T>& vector)
	{
		return UnitVector3<T>(0, vector.Y(), vector.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> RemoveY(const UnitVector3<T>& vector)
	{
		return UnitVector3<T>(vector.X(), 0, vector.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> RemoveZ(const UnitVector3<T>& vector)
	{
		return UnitVector3<T>(vector.X(), vector.Y(), 0);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> Min(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Vector2<T>(Min(a.x, b.x), Min(a.y, b.y));
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> Max(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Vector2<T>(Max(a.x, b.x), Max(a.y, b.y));
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> Min(const Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Vector3<T>(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> Max(const Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Vector3<T>(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> Min(const Point2<T>& a, const Point2<T>& b) noexcept
	{
		return Point2<T>(Min(a.x, b.x), Min(a.y, b.y));
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> Max(const Point2<T>& a, const Point2<T>& b) noexcept
	{
		return Point2<T>(Max(a.x, b.x), Max(a.y, b.y));
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> Min(const Point3<T>& a, const Point3<T>& b) noexcept
	{
		return Point3<T>(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> Max(const Point3<T>& a, const Point3<T>& b) noexcept
	{
		return Point3<T>(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
	}

	template<typename T>
    [[nodiscard]] constexpr T MaxComponent(const Vector2<T>& vector) noexcept
    {
        return Max(vector.x, vector.y);
    }

	template<typename T>
	[[nodiscard]] constexpr T MaxComponent(const Vector3<T>& vector) noexcept
	{
		return Max(Max(vector.x, vector.y), vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr T DistanceSquared(const Point2<T>& a, const Point2<T>& b) noexcept
	{
		return Square(a.x - b.x) + Square(a.y - b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr T DistanceSquared(const Point3<T>& a, const Point3<T>& b) noexcept
	{
		return Square(a.x - b.x) + Square(a.y - b.y) + Square(a.z - b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr T Distance(const Point2<T>& a, const Point2<T>& b) noexcept
	{
		return Sqrt(DistanceSquared(a, b));
	}

	template<typename T>
	[[nodiscard]] constexpr T Distance(const Point3<T>& a, const Point3<T>& b) noexcept
	{
		return Sqrt(DistanceSquared(a, b));
	}

	template<typename T>
	[[nodiscard]] constexpr T LengthSquared(const Vector2<T>& vector) noexcept
	{
		return LengthSquared(vector.x, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr T LengthSquared(const Vector3<T>& vector) noexcept
	{
		return LengthSquared(vector.x, vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr T Length(const Vector2<T>& vector) noexcept
	{
		return Length(vector.x, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr T Length(const Vector3<T>& vector) noexcept
	{
		return Length(vector.x, vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsNormalized(const Vector2<T>& vector) noexcept
	{
		return IsNormalized(vector.x, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsNormalized(const Vector3<T>& vector) noexcept
	{
		return IsNormalized(vector.x, vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr void Normalize(Vector2<T>& vector)
	{
		Normalize<false>(vector.x, vector.y);
	}

	template<typename T>
	[[nodiscard]] constexpr void Normalize(Vector3<T>& vector)
	{
		Normalize<false>(vector.x, vector.y, vector.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> ToNormalized(const Vector2<T>& vector)
	{
		Vector2<T> result = vector;
		Normalize(result);
		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> ToNormalized(const Vector3<T>& vector)
	{
		Vector3<T> result = vector;
		Normalize(result);
		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Dot(a.x, a.y, b.x, b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const UnitVector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return Dot(a.X(), a.Y(), b.X(), b.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const Vector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return Dot(a.x, a.y, b.X(), b.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const UnitVector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Dot(b, a);
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const Vector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Dot(a.x, a.y, a.z, b.x, b.y, b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const UnitVector3<T>& a, const UnitVector3<T>& b) noexcept
	{
		return Dot(a.X(), a.Y(), a.Z(), b.X(), b.Y(), b.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const UnitVector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Dot(a.X(), a.Y(), a.Z(), b.x, b.y, b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr T Dot(const Vector3<T>& a, const UnitVector3<T>& b) noexcept
	{
		return Dot(b, a);
	}

	template<typename T>
	[[nodiscard]] constexpr T Cross(const Vector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Cross(a.x, a.y, b.x, b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr T Cross(const UnitVector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return Cross(a.X(), a.Y(), b.X(), b.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> Cross(const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(Cross(a.x, a.y, a.z, b.x, b.y, b.z));
	}

	template<template<typename> typename Ret = UnitVector3, typename T>
	[[nodiscard]] constexpr Ret<T> Cross(const UnitVector3<T>& a, const UnitVector3<T>& b)
	{
		return Ret<T>(Cross(a.X(), a.Y(), a.Z(), b.X(), b.Y(), b.Z()));
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> Cross(const Vector3<T>& a, const UnitVector3<T>& b)
	{
		return Vector3<T>(Cross(a.x, a.y, a.z, b.X(), b.Y(), b.Z()));
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> Cross(const UnitVector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(Cross(a.X(), a.Y(), a.Z(), b.x, b.y, b.z));
	}

	template<typename T, typename U, typename V>
	[[nodiscard]] constexpr Point3<T> InterpPoint(const Point3<T>& current, const Point3<T>& target, const U interpSpeed, const V deltaTime)
	{
		Vector3<T> deltaVector = target - current;
		const T deltaVectorLengthSquared = LengthSquared(deltaVector);
		const auto maxStep = interpSpeed * deltaTime;

		if (deltaVectorLengthSquared <= Square(maxStep))
		{
			return target;
		}

        // Manual normalization to avoid double Square calculation
		const T length = Sqrt(deltaVectorLengthSquared);
        deltaVector.x /= length;
        deltaVector.y /= length;
        deltaVector.z /= length;

		return current + deltaVector * maxStep;
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> CalculateAngle(const UnitVector2<T>& a, const UnitVector2<T>& b)
	{
		const T dotProduct = Dot(a, b);
		const T crossProduct = Cross(b, a);

		const Radians<T> angleRadians = ACos(dotProduct);

		if (crossProduct < 0)
		{
			return -angleRadians; // Flip the sign for clockwise rotations
		}

		return angleRadians;
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> CalculateAngle(Vector2<T> a, Vector2<T> b)
	{
		Normalize(a);
		Normalize(b);
		const T dotProduct = Dot(a, b);
		const T crossProduct = Cross(b, a);

		const Radians<T> angleRadians = ACos(dotProduct);

		if (crossProduct < 0)
		{
			return -angleRadians; // Flip the sign for clockwise rotations
		}

		return angleRadians;
	}

	// 
	template<typename T>
    [[nodiscard]] constexpr Radians<T> CalculateAngleNew(const UnitVector2<T>& a, const UnitVector2<T>& b)
	{
		const T dot = Dot(a, b);
		const T cross = Cross(a, b);
		const Radians<T> angleRadians = ATan2(cross, dot);
		return angleRadians;
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> CalculateAngle(const UnitVector3<T>& a, const UnitVector3<T>& b)
	{
		return ACos(Dot(a, b));
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> CalculateSignedAngle(const UnitVector3<T>& a, const UnitVector3<T>& b, const UnitVector3<T>& referenceUp)
	{
		const T dotProd = Dot(a, b);
		const UnitVector3<T> crossProd = Cross(a, b);

		const Radians<T> angle = ACos(dotProd);

		const T sign = Sign(Dot(crossProd, referenceUp));

		return angle * sign;
	}

	template<typename T>
	[[nodiscard]] constexpr Radians<T> CalculateSignedAngle(Vector3<T> a, Vector3<T> b, const UnitVector3<T>& referenceUp)
	{
		Normalize(a);
		Normalize(b);
		const T dotProd = Dot(a, b);
		const UnitVector3<T> crossProd = Cross(a, b);

		const Radians<T> angle = ACos(dotProd);

		const T sign = Sign(Dot(crossProd, referenceUp));

		return angle * sign;
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInRange(const Point2<T>& a, const Point2<T>& b, const T& range) noexcept
	{
		return DistanceSquared(a, b) <= Square(range);
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsInRange(const Point3<T>& a, const Point3<T>& b, const T& range) noexcept
	{
		return DistanceSquared(a, b) <= Square(range);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> GetUnitVector(const Point2<T>& from, const Point2<T>& to)
	{
		return UnitVector2<T>(to - from);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetUnitVector(const Point3<T>& from, const Point3<T>& to)
	{
		return UnitVector3<T>(to - from);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetReflectionVector(const UnitVector3<T>& direction, const UnitVector3<T>& normal)
	{
		const T dotProduct = Dot(direction, normal);
		const Vector3<T> projection = normal * (2 * dotProduct);
		return UnitVector3<T>(direction - projection);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetReflectionVector(const Vector3<T>& direction, const UnitVector3<T>& normal)
	{
		return GetReflectionVector(UnitVector3<T>(direction), normal);
	}

	// Rotates left
	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> GetPerpendicularVector(const Vector2<T>& vector)
	{
		return UnitVector2<T>(-vector.y, vector.x);
	}

	// Rotates left
	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> GetPerpendicularVector(const UnitVector2<T>& vector)
	{
		return UnitVector2<T>(-vector.Y(), vector.X());
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetPerpendicularVector(const UnitVector3<T>& vector, const UnitVector3<T>& normal)
	{
		return Cross(vector, normal);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> GetPerpendicularVector(const Vector3<T>& vector, const UnitVector3<T>& normal)
	{
		return Cross(vector, normal);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetPerpendicularVector(const UnitVector3<T>& vector)
	{
		const UnitVector3<T> normal = (Abs(vector.X()) < T{ 0.99 }) ? UnitVector3<T>::Right() : UnitVector3<T>::Up();
		return GetPerpendicularVector(vector, normal);
	}

	[[nodiscard]] constexpr Point2f ToClipCoords(const Point2i& point, const Dimension2u& size)
	{
		return Remap(Point2f(point), Point2f::Zero(), Point2f::Zero() + static_cast<Vector2f>(ToVector2(size)), -Point2f::One(), Point2f::One());
	}

	template<typename T, typename... Points>
	[[nodiscard]] constexpr Point2<T> AveragePoint(const Point2<T>& point, Points&&... points) requires(std::conjunction_v<std::is_same<Point2<T>, std::remove_cvref_t<Points>>...>)
	{
		Point2<T> result = point;
		const std::size_t count = sizeof...(Points) + 1;
		((result.x += points.x), ...);
		((result.y += points.y), ...);
		result.x /= count;
		result.y /= count;
		return result;
	}


	template<typename T, typename... Points>
	[[nodiscard]] constexpr Point3<T> AveragePoint(const Point3<T>& point, Points&&... points) requires(std::conjunction_v<std::is_same<Point3<T>, std::remove_cvref_t<Points>>...>)
	{
		Point3<T> result = point;
		const std::size_t count = sizeof...(Points) + 1;
		((result.x += points.x), ...);
		((result.y += points.y), ...);
		((result.z += points.z), ...);
		result.x /= count;
		result.y /= count;
		result.z /= count;
		return result;
	}

	template<typename Ret = float, typename T>
	constexpr float ToAspectRatio(const Dimension2<T>& resolution)
	{
        ASSERT(resolution.GetWidth() != 0 && "Resolution height cannot be zero when calculating aspect ratio.");
		return static_cast<float>(resolution.GetWidth()) / static_cast<float>(resolution.GetHeight());
	}
}