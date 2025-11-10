#pragma once
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Math/Rotator.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/Angle.hpp"

namespace Simple
{

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> ToWorldSpace(const Matrix4x4<T>& local, const Matrix4x4<T>& world)
	{
		return local * world;
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> ToLocalSpace(const Matrix4x4<T>& worldSpace, const Matrix4x4<T>& parent)
	{
		return worldSpace * Matrix4x4<T>::GetFastInverse(parent);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> ToWorldSpace(const Vector3<T>& local, const Matrix4x4<T>& parent)
	{
		return local * parent.GetRotationMatrix();
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateRotationMatrix(const Rotator<T>& rotator)
	{
		Matrix4x4<T> rotationMatrix = Matrix4x4<T>::Identity();
		rotationMatrix *= Matrix4x4<T>::CreateRotationAroundX(rotator.Pitch());
		rotationMatrix *= Matrix4x4<T>::CreateRotationAroundY(rotator.Yaw());
		rotationMatrix *= Matrix4x4<T>::CreateRotationAroundZ(rotator.Roll());
		return rotationMatrix;
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromAxesUnchecked(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis, const UnitVector3<T>& zAxis)
	{
		return Matrix4x4<T>({
				xAxis.X(), xAxis.Y(), xAxis.Z(), T(0),
				yAxis.X(), yAxis.Y(), yAxis.Z(), T(0),
				zAxis.X(), zAxis.Y(), zAxis.Z(), T(0),
				T(0), T(0), T(0), T(1)
			});
	}

	// Prioritzes X over Y
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis)
	{
		const UnitVector3<T> zAxis = Cross(xAxis, yAxis);
		const UnitVector3<T> yAxisCorrected = Cross(zAxis, xAxis);
		return CreateMatrixFromAxesUnchecked(xAxis, yAxisCorrected, zAxis);
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromXZ(const UnitVector3<T>& xAxis, const UnitVector3<T>& zAxis)
	{
		const UnitVector3<T> yAxis = Cross(zAxis, xAxis);
		const UnitVector3<T> zAxisCorrected = Cross(xAxis, yAxis);
		return CreateMatrixFromAxesUnchecked(xAxis, yAxis, zAxisCorrected);
	}

	// Prioritzes Y over Z
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromYX(const UnitVector3<T>& yAxis, const UnitVector3<T>& xAxis)
	{
		const UnitVector3<T> zAxis = Cross(xAxis, yAxis);
		const UnitVector3<T> xAxisCorrected = Cross(yAxis, zAxis);
		return CreateMatrixFromAxesUnchecked(xAxisCorrected, yAxis, zAxis);
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromYZ(const UnitVector3<T>& yAxis, const UnitVector3<T>& zAxis)
	{
		const UnitVector3<T> xAxis = Cross(yAxis, zAxis);
		const UnitVector3<T> zAxisCorrected = Cross(xAxis, yAxis);
		return CreateMatrixFromAxesUnchecked(xAxis, yAxis, zAxisCorrected);
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromX(const UnitVector3<T>& xAxis)
	{
		const UnitVector3<T> yAxis = GetPerpendicularVector(xAxis);
		return CreateMatrixFromXY(xAxis, yAxis);
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromY(const UnitVector3<T>& yAxis)
	{
		const UnitVector3<T> zAxis = GetPerpendicularVector(yAxis);
		return CreateMatrixFromYZ(yAxis, zAxis);
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromZ(const UnitVector3<T>& zAxis)
	{
		const UnitVector3<T> xAxis = GetPerpendicularVector(zAxis);
		return CreateMatrixFromXZ(xAxis, zAxis);
	}

	template<typename T>
	[[nodiscard]] constexpr Rotator<T> ToRotator(const Matrix4x4<T>& matrix)
	{
		const UnitVector3<T> xAxis = matrix.GetRight();
		const UnitVector3<T> yAxis = matrix.GetUp();
		const UnitVector3<T> zAxis = matrix.GetForward();

		const Radians<T> rotXangle = ATan2(-yAxis.Z(), zAxis.Z());
		const T cosYangle = Sqrt(Square(xAxis.X()) + Square(xAxis.Y()));
		const Radians<T> rotYangle = ATan2(xAxis.Z(), cosYangle);
		const T sinXangle = Sin(rotXangle);
		const T cosXangle = Cos(rotXangle);
		const Radians<T> rotZangle = ATan2(cosXangle * yAxis.X() + sinXangle * zAxis.X(), cosXangle * yAxis.Y() + sinXangle * zAxis.Y());

		return Rotator<T>(-rotXangle, -rotYangle, -rotZangle);
	}

	template<typename T>
	[[nodiscard]] constexpr Rotator<T> CreateRotatorFromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis)
	{
		return ToRotator(CreateMatrixFromXY(xAxis, yAxis));
	}

	template<typename T>
	[[nodiscard]] constexpr Rotator<T> CreateRotatorFromXZ(const UnitVector3<T>& xAxis, const UnitVector3<T>& zAxis)
	{
		return ToRotator(CreateMatrixFromXZ(xAxis, zAxis));
	}

	template<typename T>
	[[nodiscard]] constexpr Rotator<T> CreateRotatorFromYZ(const UnitVector3<T>& yAxis, const UnitVector3<T>& zAxis)
	{
		return ToRotator(CreateMatrixFromYZ(yAxis, zAxis));
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> RotateVectorAroundAxis(const Vector3<T>& vector, const UnitVector3<T>& axis, const Radians<T> angle)
	{
		const T cosTheta = Cos(angle);
		const T sinTheta = Sin(angle);

		const Vector3<T> k = { axis.X(), axis.Y(), axis.Z() };

		const Vector3<T> term1 = vector * cosTheta;
		const Vector3<T> term2 = Cross(k, vector) * sinTheta;
		const Vector3<T> term3 = k * (Dot(k, vector)) * (1 - cosTheta);

		return term1 + term2 + term3;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> RotateVectorAroundAxis(const Vector3<T>& vector, const UnitVector3<T>& axis, const Degrees<T> angle)
	{
		return RotateVectorAroundAxis(vector, axis, ToRadians(angle));
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> RotateVectorAroundAxis(const UnitVector3<T>& vector, const UnitVector3<T>& axis, const Radians<T> angle)
	{
		return UnitVector3<T>(RotateVectorAroundAxis(vector * 1.f, axis, angle));
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> RotateVectorAroundAxis(const UnitVector3<T>& vector, const UnitVector3<T>& axis, const Degrees<T> angle)
	{
		return RotateVectorAroundAxis(vector, axis, ToRadians(angle));
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> RotatePointAroundAxis(const Point3<T>& rotateAroundPoint, const Point3<T>& point, const UnitVector3<T>& axis, const Radians<T> angle)
	{
		return rotateAroundPoint + RotateVectorAroundAxis(point - rotateAroundPoint, axis, angle);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> RotatePointAroundAxis(const Point3<T>& rotateAroundPoint, const Point3<T>& point, const UnitVector3<T>& axis, const Degrees<T> angle)
	{
		return RotatePointAroundAxis(rotateAroundPoint, point, axis, ToRadians(angle));
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateRotationAroundAxis(const UnitVector3<T>& axis, Radians<T> angle)
	{
		const T x = axis.X();
		const T y = axis.Y();
		const T z = axis.Z();

		const T c = Cos(angle);
		const T s = Sin(angle);
		const T t = static_cast<T>(1) - c;

		Matrix4x4<T> result = Matrix4x4<T>::Identity();

		result(1, 1) = t * x * x + c;
		result(1, 2) = t * x * y - s * z;
		result(1, 3) = t * x * z + s * y;

		result(2, 1) = t * x * y + s * z;
		result(2, 2) = t * y * y + c;
		result(2, 3) = t * y * z - s * x;

		result(3, 1) = t * x * z - s * y;
		result(3, 2) = t * y * z + s * x;
		result(3, 3) = t * z * z + c;

		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> GetRotatedMatrixAroundAxis(const Matrix4x4<T>& matrix, const UnitVector3<T>& axis, const Radians<T>& angle)
	{
		Matrix4x4<T> m = matrix;
		const Point3<T> previousTranslation = m.GetTranslation();
		m.SetTranslation(Point3f::Zero());
		m = m * CreateRotationAroundAxis(axis, angle);
		m.SetTranslation(previousTranslation);
		return m;
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> GetRotatedMatrixAroundAxis(const Matrix4x4<T>& matrix, const UnitVector3<T>& axis, const Degrees<T>& angle)
	{
		return GetRotatedMatrixAroundAxis(matrix, axis, ToRadians(angle));
	}

	template<typename T>
	constexpr void RotateMatrixAroundAxis(Matrix4x4<T>& matrix, const UnitVector3<T>& axis, const Radians<T>& angle)
	{
		matrix = GetRotatedMatrixAroundAxis(matrix, axis, angle);
	}

	template<typename T>
	constexpr void RotateMatrixAroundAxis(Matrix4x4<T>& matrix, const UnitVector3<T>& axis, const Degrees<T>& angle)
	{
		RotateMatrixAroundAxis(matrix, axis, ToRadians(angle));
	}

	template<typename T>
	constexpr void RotateMatrixAroundY(Matrix4x4<T>& matrix, const Degrees<T>& angle)
	{
		Matrix4x4<T> m = matrix;
		const Point3<T> p = m.GetTranslation();
		m.SetTranslation(Point3<T>::Zero());
		m = Matrix4x4<T>::CreateRotationAroundY(angle) * m;
		m.SetTranslation(p);
		matrix = m;
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetRotatorRight(const Rotator<T>& rotator)
	{
		return CreateRotationMatrix(rotator).GetRight();
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetRotatorUp(const Rotator<T>& rotator)
	{
		return CreateRotationMatrix(rotator).GetUp();
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> GetRotatorForward(const Rotator<T>& rotator)
	{
		return CreateRotationMatrix(rotator).GetForward();
	}

	template<typename T>
	[[nodiscard]] constexpr Rotator<T> GetNormalized360(const Rotator<T>& rotator)
	{
		Rotator<T> normalizedRotator = rotator;
		normalizedRotator.Normalize360();
		return normalizedRotator;
	}

	template<typename T>
	[[nodiscard]] constexpr Rotator<T> GetNormalized180(const Rotator<T>& rotator)
	{
		Rotator<T> normalizedRotator = rotator;
		normalizedRotator.Normalize180();
		return normalizedRotator;
	}

	template<typename T>
	constexpr Quaternion<T> Lerp(const Quaternion<T>& from, const Quaternion<T>& to, const T& delta)
	{
		Quaternion<T> result;

		const float deltaInverse = 1 - delta;

		result.w = deltaInverse * from.w + delta * to.w;

		result.x = deltaInverse * from.x + delta * to.x;
		result.y = deltaInverse * from.y + delta * to.y;
		result.z = deltaInverse * from.z + delta * to.z;

		result.Normalize();

		return result;
	}

	template<typename T>
	constexpr Quaternion<T> Slerp(const Quaternion<T>& from, const Quaternion<T>& to, const T& delta)
	{
		Quaternion<T> qz = to;

		T cosTheta = from.Dot(to);

		if (cosTheta < T(0))
		{
			qz = -qz;
			cosTheta = -cosTheta;
		}

		const T dotThreshold = static_cast<T>(0.9995);

		if (cosTheta > T(1) - dotThreshold)
		{
			return Lerp(from, qz, delta);
		}

		const auto angle = ACos(cosTheta);
		return (Sin((T(1) - delta) * angle) * from + Sin(delta * angle) * qz) / Sin(angle);
	}
}