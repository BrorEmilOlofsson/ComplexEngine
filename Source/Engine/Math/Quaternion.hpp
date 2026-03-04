#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Rotator.hpp"
#include "Engine/Math/Math.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include <utility>
#include <cassert>

namespace CLX
{
	template<typename T>
	class Quaternion final
	{
	public:
		T w = 1;
		T x = 0;
		T y = 0;
		T z = 0;

		constexpr Quaternion() = default;
		constexpr Quaternion(const T& w, const T& x, const T& y, const T& z);
		constexpr Quaternion(const Rotator<T>& aAngleInRadian);

		[[nodiscard]] static constexpr Quaternion FromAxisAndAngle(const UnitVector3<T>& axis, const Radians<T> angle);
		[[nodiscard]] static constexpr Quaternion FromAxisAndAngle(const UnitVector3<T>& axis, const Degrees<T> angle);

		constexpr void Normalize();
		constexpr Quaternion<T> GetNormalized() const;

		constexpr T Length() const;
		constexpr T LengthSquared() const;
		constexpr T Dot(const Quaternion<T>& aQuaternion) const;

		constexpr Vector3<T> GetForward() const;
		constexpr Vector3<T> GetRight() const;
		constexpr Vector3<T> GetUp() const;
		constexpr Rotator<T> GetRotator() const;

		[[nodiscard]] static constexpr Quaternion<T> Identity() noexcept;
		static constexpr Vector3<T> RotateVectorByQuaternion(const Quaternion<T>& aQuaternion, const Vector3<T>& aVectorToRotate);
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;

	template<typename T>
	constexpr Quaternion<T>::Quaternion(const T& w, const T& x, const T& y, const T& z)
		: w(w)
		, x(x)
		, y(y)
		, z(z)
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
	constexpr Quaternion<T> Quaternion<T>::FromAxisAndAngle(const UnitVector3<T>& axis, const Radians<T> angle)
	{
		const Radians<T> halfAngle = angle / T(2);
		const T w = Cos(halfAngle);
		const T halfAngleSin = Sin(halfAngle);
		const T x = axis.X() * halfAngleSin;
		const T y = axis.Y() * halfAngleSin;
		const T z = axis.Z() * halfAngleSin;
		return Quaternion<T>(w, x, y, z);
	}

	template<typename T>
	constexpr Quaternion<T> Quaternion<T>::FromAxisAndAngle(const UnitVector3<T>& axis, const Degrees<T> angle)
	{
		return FromAxisAndAngle(axis, ToRadians(angle));
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
	constexpr Vector3<T> Quaternion<T>::GetRight() const
	{
		constexpr Vector3<T> right(1.0f, 0.0f, 0.0f);
		return Quaternion<T>::RotateVectorByQuaternion(*this, right);
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetUp() const
	{
		constexpr Vector3<T> up(0.0f, 1.0f, 0.0f);
		return Quaternion<T>::RotateVectorByQuaternion(*this, up);
	}

	template<typename T>
	constexpr Vector3<T> Quaternion<T>::GetForward() const
	{
		constexpr Vector3<T> forward(0.0f, 0.0f, 1.0f);
		return Quaternion<T>::RotateVectorByQuaternion(*this, forward);
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
	constexpr Quaternion<T> Quaternion<T>::Identity() noexcept
	{
		return Quaternion<T>(1, 0, 0, 0);
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
	Quaternion<T>& operator*=(Quaternion<T>& aQuaternion, const T& aScalar)
	{
		aQuaternion.w *= aScalar;
		aQuaternion.x *= aScalar;
		aQuaternion.y *= aScalar;
		aQuaternion.z *= aScalar;
		return aQuaternion;
	}

	template<class T>
	Quaternion<T>& operator*=(Quaternion<T>& aQuaternionA, const Quaternion<T>& aQuaternionB)
	{
		const T w = aQuaternionA.w;
		const T x = aQuaternionA.x;
		const T y = aQuaternionA.y;
		const T z = aQuaternionA.z;

		aQuaternionA.w = (aQuaternionB.w * w) - (aQuaternionB.x * x) - (aQuaternionB.y * y) - (aQuaternionB.z * z);
		aQuaternionA.x = (aQuaternionB.w * x) + (aQuaternionB.x * w) + (aQuaternionB.y * z) - (aQuaternionB.z * y);
		aQuaternionA.y = (aQuaternionB.w * y) + (aQuaternionB.y * w) + (aQuaternionB.z * x) - (aQuaternionB.x * z);
		aQuaternionA.z = (aQuaternionB.w * z) + (aQuaternionB.z * w) + (aQuaternionB.x * y) - (aQuaternionB.y * x);
		return aQuaternionA;
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
	[[nodiscard]] constexpr bool NearlyEqual(const Quaternion<T>& a, const Quaternion<T>& b, const T& epsilon = std::numeric_limits<T>::epsilon())
	{
		// Quaternions q and -q represent the same rotation.
		return (
			(Abs(a.x - b.x) < epsilon &&
				Abs(a.y - b.y) < epsilon &&
				Abs(a.z - b.z) < epsilon &&
				Abs(a.w - b.w) < epsilon)
			||
			(Abs(a.x + b.x) < epsilon &&
				Abs(a.y + b.y) < epsilon &&
				Abs(a.z + b.z) < epsilon &&
				Abs(a.w + b.w) < epsilon)
			);
	}

}