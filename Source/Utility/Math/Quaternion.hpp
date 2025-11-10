#pragma once
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Rotator.hpp"
#include "Utility/Math/Math.hpp"
#include <iostream>
#include <utility>

#undef max
//Work In Progress

namespace Simple
{
	template <typename T>
	class Matrix4x4;

	template<typename T>
	class Quaternion final
	{
	public:
		T w = 1;
		T x = 0;
		T y = 0;
		T z = 0;

		constexpr Quaternion() = default;
		constexpr Quaternion(const T& aW, const T& aX, const T& aY, const T& aZ);
		constexpr Quaternion(const Rotator<T>& aAngleInRadian);
		constexpr Quaternion(const Vector3<T>& aAxis, const T aAngle);
		constexpr Quaternion(const Matrix4x4<T>& aMatrix);

		constexpr void Normalize();
		constexpr Quaternion<T> GetNormalized() const;

		constexpr T Length() const;
		constexpr T LengthSquared() const;
		constexpr T Dot(const Quaternion<T>& aQuaternion) const;

		constexpr Matrix4x4<T> GetRotationMatrix4x4() const;
		constexpr Vector3<T> GetForward() const;
		constexpr Vector3<T> GetRight() const;
		constexpr Vector3<T> GetUp() const;
		constexpr Rotator<T> GetRotator() const;

		constexpr static Vector3<T> RotateVectorByQuaternion(const Quaternion<T>& aQuaternion, const Vector3<T>& aVectorToRotate);
	};

	typedef Quaternion<float> Quaternionf;

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const T& aW, const T& aX, const T& aY, const T& aZ)
		: w(aW)
		, x(aX)
		, y(aY)
		, z(aZ)
	{
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Rotator<T>& aAngleInRadian)
	{
		const T cx = Cos(aAngleInRadian.Pitch() * T(0.5));
		const T cy = Cos(aAngleInRadian.Yaw() * T(0.5));
		const T cz = Cos(aAngleInRadian.Roll() * T(0.5));
		const T sx = Sin(aAngleInRadian.Pitch() * T(0.5));
		const T sy = Sin(aAngleInRadian.Yaw() * T(0.5));
		const T sz = Sin(aAngleInRadian.Roll() * T(0.5));

		w = cx * cy * cz + sx * sy * sz;

		x = sx * cy * cz - cx * sy * sz;
		y = cx * sy * cz + sx * cy * sz;
		z = cx * cy * sz - sx * sy * cz;
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Vector3<T>& aAxis, const T aAngle)
	{
		const T halfAngle = aAngle / T(2);

		w = std::cos(halfAngle);

		const T halfAngleSin = sin(halfAngle);

		x = aAxis.x * halfAngleSin;
		y = aAxis.y * halfAngleSin;
		z = aAxis.z * halfAngleSin;
	}

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const Matrix4x4<T>& aMatrix)
	{
		w = std::sqrt(std::max(T(0), T(1) + aMatrix(1, 1) + aMatrix(2, 2) + aMatrix(3, 3))) * T(0.5);

		x = std::sqrt(std::max(T(0), T(1) + aMatrix(1, 1) - aMatrix(2, 2) - aMatrix(3, 3))) * T(0.5);
		y = std::sqrt(std::max(T(0), T(1) - aMatrix(1, 1) + aMatrix(2, 2) - aMatrix(3, 3))) * T(0.5);
		z = std::sqrt(std::max(T(0), T(1) - aMatrix(1, 1) - aMatrix(2, 2) + aMatrix(3, 3))) * T(0.5);

		x = std::copysign(x, aMatrix(3, 2) - aMatrix(2, 3));
		y = std::copysign(y, aMatrix(1, 3) - aMatrix(3, 1));
		z = std::copysign(z, aMatrix(2, 1) - aMatrix(1, 2));
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& aOS, const Quaternion<T>& aQuaternion)
	{
		return aOS << "{ W: " << aQuaternion.w << " X: " << aQuaternion.x << " Y: " << aQuaternion.y << " Z: " << aQuaternion.z << " }" << std::endl;
	}

	template<typename T>
	constexpr Quaternion<T> operator*(const Quaternion<T>& aQuaternion, const T& aScalar)
	{
		return Quaternion<T>(aQuaternion.w * aScalar, aQuaternion.x * aScalar, aQuaternion.y * aScalar, aQuaternion.z * aScalar);
	}

	template<typename T>
	constexpr Quaternion<T> operator*(const T& aScalar, const Quaternion<T>& aQuaternion)
	{
		return Quaternion<T>(aQuaternion.w * aScalar, aQuaternion.x * aScalar, aQuaternion.y * aScalar, aQuaternion.z * aScalar);
	}

	template<class T>
	Quaternion<T> operator*(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		return Quaternion<T>(
			(aQuaternionB.w * aQuaternionA.w) - (aQuaternionB.x * aQuaternionA.x) - (aQuaternionB.y * aQuaternionA.y) - (aQuaternionB.z * aQuaternionA.z),
			(aQuaternionB.w * aQuaternionA.x) + (aQuaternionB.x * aQuaternionA.w) + (aQuaternionB.y * aQuaternionA.z) - (aQuaternionB.z * aQuaternionA.y),
			(aQuaternionB.w * aQuaternionA.y) + (aQuaternionB.y * aQuaternionA.w) + (aQuaternionB.z * aQuaternionA.x) - (aQuaternionB.x * aQuaternionA.z),
			(aQuaternionB.w * aQuaternionA.z) + (aQuaternionB.z * aQuaternionA.w) + (aQuaternionB.x * aQuaternionA.y) - (aQuaternionB.y * aQuaternionA.x)
		);
	}

	template<class T>
	void operator*=(Quaternion<T>& aQuaternion, const T& aScalar)
	{
		aQuaternion.w *= aScalar;
		aQuaternion.x *= aScalar;
		aQuaternion.y *= aScalar;
		aQuaternion.z *= aScalar;
	}

	template<class T>
	void operator*=(Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		const T w = aQuaternionA.w;
		const T x = aQuaternionA.x;
		const T y = aQuaternionA.y;
		const T z = aQuaternionA.z;

		aQuaternionA.w = (aQuaternionB.w * w) - (aQuaternionB.x * x) - (aQuaternionB.y * y) - (aQuaternionB.z * z);
		aQuaternionA.x = (aQuaternionB.w * x) + (aQuaternionB.x * w) + (aQuaternionB.y * z) - (aQuaternionB.z * y);
		aQuaternionA.y = (aQuaternionB.w * y) + (aQuaternionB.y * w) + (aQuaternionB.z * x) - (aQuaternionB.x * z);
		aQuaternionA.z = (aQuaternionB.w * z) + (aQuaternionB.z * w) + (aQuaternionB.x * y) - (aQuaternionB.y * x);
	}

	template<typename T>
	Quaternion<T> operator/(const Quaternion<T>& aQuaternion, const T& aScalar)
	{
		return Quaternion<T>(aQuaternion.w / aScalar, aQuaternion.x / aScalar, aQuaternion.y / aScalar, aQuaternion.z / aScalar);
	}

	template<typename T>
	Quaternion<T> operator-(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		return Quaternion<T>(aQuaternionA.w - aQuaternionB.w, aQuaternionA.x - aQuaternionB.x, aQuaternionA.y - aQuaternionB.y, aQuaternionA.z - aQuaternionB.z);
	}

	template<typename T>
	Quaternion<T> operator-(const Quaternion<T>& aQuaternion)
	{
		return Quaternion<T>(-aQuaternion.w, -aQuaternion.x, -aQuaternion.y, -aQuaternion.z);
	}

	template<class T>
	Quaternion<T> operator+(const Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		return Quaternion<T>(aQuaternionA.w + aQuaternionB.w, aQuaternionA.x + aQuaternionB.x, aQuaternionA.y + aQuaternionB.y, aQuaternionA.z + aQuaternionB.z);
	}

	template<typename T>
	Quaternion<T>& operator+=(Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		aQuaternionA.w += aQuaternionB.w;
		aQuaternionA.x += aQuaternionB.x;
		aQuaternionA.y += aQuaternionB.y;
		aQuaternionA.z += aQuaternionB.z;
		return aQuaternionA;
	}

	template<typename T>
	constexpr void Quaternion<T>::Normalize()
	{
		const T length = T(1) / Length();

		w *= length;

		x *= length;
		y *= length;
		z *= length;
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::GetNormalized() const
	{
		const T length = T(1) / Length();

		return Quaternion<T>(w * length, x * length, y * length, z * length);
	}

	template<typename T>
	constexpr T Quaternion<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template<typename T>
	constexpr T Quaternion<T>::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}

	template<typename T>
	constexpr T Quaternion<T>::Dot(const Quaternion<T>& aQuaternion) const
	{
		return x * aQuaternion.x + y * aQuaternion.y + z * aQuaternion.z + w * aQuaternion.w;
	}

	template<typename T>
	constexpr Matrix4x4<T> Quaternion<T>::GetRotationMatrix4x4() const
	{
		Matrix4x4<T> result;

		const T qxx(x * x);
		const T qyy(y * y);
		const T qzz(z * z);

		const T qxz(x * z);
		const T qxy(x * y);
		const T qyz(y * z);

		const T qwx(w * x);
		const T qwy(w * y);
		const T qwz(w * z);

		result(1, 1) = T(1) - T(2) * (qyy + qzz);
		result(1, 2) = T(2) * (qxy + qwz);
		result(1, 3) = T(2) * (qxz - qwy);

		result(2, 1) = T(2) * (qxy - qwz);
		result(2, 2) = T(1) - T(2) * (qxx + qzz);
		result(2, 3) = T(2) * (qyz + qwx);

		result(3, 1) = T(2) * (qxz + qwy);
		result(3, 2) = T(2) * (qyz - qwx);
		result(3, 3) = T(1) - T(2) * (qxx + qyy);

		return result;
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetForward() const
	{
		const Vector3<T> forward(0.0f, 0.0f, 1.0f);
		return Quaternion<T>::RotateVectorByQuaternion(*this, forward);
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetRight() const
	{
		const Vector3<T> right(1.0f, 0.0f, 0.0f);
		return Quaternion<T>::RotateVectorByQuaternion(*this, right);
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetUp() const
	{
		const Vector3<T> up(0.0f, 1.0f, 0.0f);
		return Quaternion<T>::RotateVectorByQuaternion(*this, up);
	}

	template<typename T>
	constexpr Rotator<T> Quaternion<T>::GetRotator() const
	{
		//Z-axis rotation
		const T sinr = T(2.0) * (w * x + y * z);
		const T cosr = T(1.0f) - T(2.0) * (x * x + y * y);
		const Radians<T> pitch = ATan2(sinr, cosr);

		//X-axis rotation
		const T sinp = T(2.0) * (w * y - z * x);
		Radians<T> yaw(T(0.0));

		if (Abs(sinp) >= T(1))
		{
			yaw = Radians<T>(std::copysign(PI<T> *T(0.5), sinp));
		}
		else
		{
			yaw = ASin(sinp);
		}

		//Y-axis rotation
		const T siny = T(2.0) * (w * z + x * y);
		const T cosy = T(1.0) - T(2.0) * (y * y + z * z);
		const Radians<T> roll = ATan2(siny, cosy);

		return Rotator<T>(pitch, yaw, roll); //I may call them wrong cuz I am confused by pitch yaw roll but it's {x y z}-axis and I do not want to confuse with this class xyz
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::RotateVectorByQuaternion(const Quaternion<T>& aQuaternion, const Vector3<T>& aVectorToRotate)
	{
		const Vector3<T> vector(aQuaternion.x, aQuaternion.y, aQuaternion.z);

		const Vector3<T> result = 2.0f * vector.Dot(aVectorToRotate) * vector
			+ (aQuaternion.w * aQuaternion.w - vector.Dot(vector)) * aVectorToRotate
			+ 2.0f * aQuaternion.w * vector.Cross(aVectorToRotate);

		return result;
	}
}