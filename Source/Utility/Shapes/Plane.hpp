#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/UnitVector3.hpp"

namespace Simple
{

	template<typename T>
	class Plane final
	{
	public:

		constexpr Plane() = default;
		constexpr Plane(const Point3<T>& point0, const Point3<T>& point1, const Point3<T>& point2);
		constexpr Plane(const Point3<T>& point, const UnitVector3<T>& normal);

		[[nodiscard]] constexpr Point3<T> GetPoint() const;
		[[nodiscard]] constexpr const UnitVector3<T>& GetNormal() const;

		void SetPoint(const Point3<T>& point);
		void SetNormal(const UnitVector3<T>& normal);

	private:

		Point3<T> mPoint;
		UnitVector3<T> mNormal = UnitVector3<T>::Up();
	};

	using Planef = Plane<float>;

	template<typename T>
	constexpr Plane<T>::Plane(const Point3<T>& point0, const Point3<T>& point1, const Point3<T>& point2)
		: mPoint(point0)
	{
		const Vector3<T> vector1 = point1 - point0;
		const Vector3<T> vector2 = point2 - point0;

		mNormal = UnitVector3<T>(Cross(vector1, vector2));
	}

	template<typename T>
	constexpr Plane<T>::Plane(const Point3<T>& point, const UnitVector3<T>& normal)
		: mPoint(point)
		, mNormal(normal)
	{
	}

	template<typename T>
	constexpr const UnitVector3<T>& Plane<T>::GetNormal() const
	{
		return mNormal;
	}

	template<typename T>
	constexpr Point3<T> Plane<T>::GetPoint() const
	{
		return mPoint;
	}

	template<typename T>
	void Plane<T>::SetPoint(const Point3<T>& point)
	{
		mPoint = point;
	}

	template<typename T>
	void Plane<T>::SetNormal(const UnitVector3<T>& normal)
	{
		mNormal = normal;
	}
}