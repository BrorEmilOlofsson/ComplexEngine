#pragma once
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Math/Quaternion.hpp"
#include "Utility/Math/Rotator.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/RotationMatrix3.hpp"
#include "Utility/Math/Angle.hpp"
#include <array>
#include <cassert>

namespace Simple
{

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> ToMatrix(const Quaternion<T>& quaternion)
	{
		Matrix4x4<T> result;

		const T w = quaternion.w;
		const T x = quaternion.x;
		const T y = quaternion.y;
		const T z = quaternion.z;

		const T qxx(x * x);
		const T qyy(y * y);
		const T qzz(z * z);

		const T qxz(x * z);
		const T qxy(x * y);
		const T qyz(y * z);

		const T qwx(w * x);
		const T qwy(w * y);
		const T qwz(w * z);

		result(0, 0) = T(1) - T(2) * (qyy + qzz);
		result(0, 1) = T(2) * (qxy + qwz);
		result(0, 2) = T(2) * (qxz - qwy);

		result(1, 0) = T(2) * (qxy - qwz);
		result(1, 1) = T(1) - T(2) * (qxx + qzz);
		result(1, 2) = T(2) * (qyz + qwx);

		result(2, 0) = T(2) * (qxz + qwy);
		result(2, 1) = T(2) * (qyz - qwx);
		result(2, 2) = T(1) - T(2) * (qxx + qyy);

		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr RotationMatrix3<T> ToRotationMatrix(const Quaternion<T>& quaternion)
	{

		const T w = quaternion.w;
		const T x = quaternion.x;
		const T y = quaternion.y;
		const T z = quaternion.z;

		const T qxx(x * x);
		const T qyy(y * y);
		const T qzz(z * z);

		const T qxz(x * z);
		const T qxy(x * y);
		const T qyz(y * z);

		const T qwx(w * x);
		const T qwy(w * y);
		const T qwz(w * z);

		std::array<T, 9> values
		{
			T(1) - T(2) * (qyy + qzz),
			T(2) * (qxy + qwz),
			T(2) * (qxz - qwy),

			T(2) * (qxy - qwz),
			T(1) - T(2) * (qxx + qzz),
			T(2) * (qyz + qwx),

			T(2) * (qxz + qwy),
			T(2) * (qyz - qwx),
			T(1) - T(2) * (qxx + qyy)
		};

		return RotationMatrix3<T>(values);
	}

	template<typename T>
	[[nodiscard]] constexpr Quaternion<T> ToQuaternion(const RotationMatrix3<T>& m)
	{
		const T trace = m(0, 0) + m(1, 1) + m(2, 2);
		Quaternion<T> q;

		if (trace > 0)
		{
			T s = Sqrt(trace + 1) * 2;
			q.w = T(0.25) * s;
			q.x = (m(2, 1) - m(1, 2)) / s;
			q.y = (m(0, 2) - m(2, 0)) / s;
			q.z = (m(1, 0) - m(0, 1)) / s;
		}
		else
		{
			if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
			{
				T s = Sqrt(T(1) + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
				q.w = (m(2, 1) - m(1, 2)) / s;
				q.x = T(0.25) * s;
				q.y = (m(0, 1) + m(1, 0)) / s;
				q.z = (m(0, 2) + m(2, 0)) / s;
			}
			else if (m(1, 1) > m(2, 2))
			{
				T s = Sqrt(T(1) + m(1, 1) - m(0, 0) - m(2, 2)) * 2;
				q.w = (m(0, 2) - m(2, 0)) / s;
				q.x = (m(0, 1) + m(1, 0)) / s;
				q.y = T(0.25) * s;
				q.z = (m(1, 2) + m(2, 1)) / s;
			}
			else
			{
				T s = Sqrt(T(1) + m(2, 2) - m(0, 0) - m(1, 1)) * 2;
				q.w = (m(1, 0) - m(0, 1)) / s;
				q.x = (m(0, 2) + m(2, 0)) / s;
				q.y = (m(1, 2) + m(2, 1)) / s;
				q.z = T(0.25) * s;
			}
		}

		q.Normalize();
		return q;
	}

	template<typename T>
	[[nodiscard]] constexpr Quaternion<T> ToQuaternion(const Matrix4x4<T>& matrix)
	{
		assert(matrix.IsOrthogonal() && "Matrix must be orthogonal to convert to Quaternion");

		return ToQuaternion(matrix.GetRotationMatrix());
	}

	template<typename T>
	constexpr Quaternion<T> ToQuaternion2(const Matrix4x4<T>& matrix)
	{
		const T trace = matrix(1, 1) + matrix(2, 2) + matrix(3, 3);
		Quaternion<T> quaternion;
		if (trace > T(0))
		{
			const T s = Sqrt(trace + T(1)) * T(2);
			quaternion.w = s * T(0.25);
			quaternion.x = (matrix(3, 2) - matrix(2, 3)) / s;
			quaternion.y = (matrix(1, 3) - matrix(3, 1)) / s;
			quaternion.z = (matrix(2, 1) - matrix(1, 2)) / s;
		}
		else if ((matrix(1, 1) > matrix(2, 2)) && (matrix(1, 1) > matrix(3, 3)))
		{
			const T s = Sqrt(T(1) + matrix(1, 1) - matrix(2, 2) - matrix(3, 3)) * T(2);
			quaternion.w = (matrix(3, 2) - matrix(2, 3)) / s;
			quaternion.x = s * T(0.25);
			quaternion.y = (matrix(1, 2) + matrix(2, 1)) / s;
			quaternion.z = (matrix(1, 3) + matrix(3, 1)) / s;
		}
		else if (matrix(2, 2) > matrix(3, 3))
		{
			const T s = Sqrt(T(1) + matrix(2, 2) - matrix(1, 1) - matrix(3, 3)) * T(2);
			quaternion.w = (matrix(1, 3) - matrix(3, 1)) / s;
			quaternion.x = (matrix(1, 2) + matrix(2, 1)) / s;
			quaternion.y = s * T(0.25);
			quaternion.z = (matrix(2, 3) + matrix(3, 2)) / s;
		}
		else
		{
			const T s = Sqrt(T(1) + matrix(3, 3) - matrix(1, 1) - matrix(2, 2)) * T(2);
			quaternion.w = (matrix(2, 1) - matrix(1, 2)) / s;
			quaternion.x = (matrix(1, 3) + matrix(3, 1)) / s;
			quaternion.y = (matrix(2, 3) + matrix(3, 2)) / s;
			quaternion.z = s * T(0.25);
		}

		return quaternion;
	}


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
	[[nodiscard]] constexpr UnitVector3<T> ToWorldSpace(const UnitVector3<T>& local, const Matrix4x4<T>& parent)
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

	// Prioritzes X-axis over Y-axis
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis)
	{
		return Matrix4x4<T>::CreateRotationMatrix(RotationMatrix3<T>::FromXY(xAxis, yAxis));
	}

	// Prioritzes X-axis over Z-axis
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromXZ(const UnitVector3<T>& xAxis, const UnitVector3<T>& zAxis)
	{
		return Matrix4x4<T>::CreateRotationMatrix(RotationMatrix3<T>::FromXZ(xAxis, zAxis));
	}

	// Prioritzes Y-axis over X-axis
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromYX(const UnitVector3<T>& yAxis, const UnitVector3<T>& xAxis)
	{
		return Matrix4x4<T>::CreateRotationMatrix(RotationMatrix3<T>::FromYX(yAxis, xAxis));
	}

	// Prioritzes Y-axis over Z-axis
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromYZ(const UnitVector3<T>& yAxis, const UnitVector3<T>& zAxis)
	{
		return Matrix4x4<T>::CreateRotationMatrix(RotationMatrix3<T>::FromYZ(yAxis, zAxis));
	}

	// Prioritzes Z-axis over X-axis
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromZX(const UnitVector3<T>& zAxis, const UnitVector3<T>& xAxis)
	{
		return Matrix4x4<T>::CreateRotationMatrix(RotationMatrix3<T>::FromZX(zAxis, xAxis));
	}

	// Prioritzes Z-axis over Y-axis
	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> CreateMatrixFromZY(const UnitVector3<T>& zAxis, const UnitVector3<T>& yAxis)
	{
		return Matrix4x4<T>::CreateRotationMatrix(RotationMatrix3<T>::FromZY(zAxis, yAxis));
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

		result(0, 0) = t * x * x + c;
		result(0, 1) = t * x * y - s * z;
		result(0, 2) = t * x * z + s * y;

		result(1, 0) = t * x * y + s * z;
		result(1, 1) = t * y * y + c;
		result(1, 2) = t * y * z - s * x;

		result(2, 0) = t * x * z - s * y;
		result(2, 1) = t * y * z + s * x;
		result(2, 2) = t * z * z + c;

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