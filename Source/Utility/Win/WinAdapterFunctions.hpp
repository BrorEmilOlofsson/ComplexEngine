#pragma once

#ifdef _WIN32
#include <DirectXMath.h>
#include <Windows.h>
#include "Utility/Color.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Point2.hpp"
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Shapes/AABB2.hpp"
 
namespace Simple
{

	[[nodiscard]] inline DirectX::XMVECTOR ToXMVector(const Color& color)
	{
		const DirectX::XMFLOAT4* colorPtr = reinterpret_cast<const DirectX::XMFLOAT4*>(&color);
		return DirectX::XMLoadFloat4(colorPtr);
	}

	[[nodiscard]] inline DirectX::XMFLOAT2 ToXMFloat2(const Vector2f& vector)
	{
		return DirectX::XMFLOAT2(vector.x, vector.y);
	}

	[[nodiscard]] inline DirectX::XMFLOAT2 ToXMFloat2(const Point2f& point)
	{
		return DirectX::XMFLOAT2(point.x, point.y);
	}

	[[nodiscard]] inline DirectX::XMFLOAT3 ToXMFloat3(const Vector3f& vector)
	{
		return DirectX::XMFLOAT3(vector.x, vector.y, vector.z);
	}

	[[nodiscard]] inline DirectX::XMFLOAT3 ToXMFloat3(const Point3f& point)
	{
		return DirectX::XMFLOAT3(point.x, point.y, point.z);
	}

	[[nodiscard]] inline DirectX::XMVECTOR ToXMVector(const Point3f& vector)
	{
		return DirectX::XMVECTOR{ vector.x, vector.y, vector.z, 1 };
	}

	[[nodiscard]] inline DirectX::XMMATRIX ToXMMATRIX(const Matrix4x4f& matrix)
	{
		return DirectX::XMMATRIX(matrix.GetDataPtr());
	}

	[[nodiscard]] inline Matrix4x4f ToMatrix4x4f(const DirectX::XMMATRIX& matrix)
	{
		Matrix4x4f result;
		static_assert(sizeof(Matrix4x4f) == sizeof(DirectX::XMMATRIX), "Size mismatch between Matrix4x4f and DirectX::XMMATRIX");
		std::memcpy(result.GetDataPtr(), &matrix, sizeof(Matrix4x4f));
		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr POINT ToPOINT(Point2<T> point)
	{
		return POINT{ static_cast<long>(point.x), static_cast<long>(point.y) };
	}

	template<typename T = int>
	[[nodiscard]] constexpr Point2<T> ToPoint(POINT point)
	{
		return Point2<T>(static_cast<T>(point.x), static_cast<T>(point.y));
	}

	template<typename T = float>
	[[nodiscard]] constexpr Point2<T> ToPoint(const DirectX::XMFLOAT2& point)
	{
		return Point2<T>(static_cast<T>(point.x), static_cast<T>(point.y));
	}

	[[nodiscard]] inline RECT ToRECT(const AABB2i64& aabb)
	{
		return RECT
		{
			.left = static_cast<LONG>(aabb.GetMin().x),
            .top = static_cast<LONG>(aabb.GetMin().y),
            .right = static_cast<LONG>(aabb.GetMax().x),
            .bottom = static_cast<LONG>(aabb.GetMax().y)
		};
	}

	[[nodiscard]] inline RECT ToRECT(const AABB2i& aabb)
	{
		return RECT
		{
			.left = aabb.GetMin().x,
			.top = aabb.GetMin().y,
			.right = aabb.GetMax().x,
			.bottom = aabb.GetMax().y
		};
	}

	template<typename T = float>
	inline AABB2<T> ToAABB(const RECT& rect)
	{
		return AABB2<T>::FromMinAndMax
		(
			Point2<T>(rect.left, rect.top),
			Point2<T>(rect.right, rect.bottom)
		);
	}
}

#endif