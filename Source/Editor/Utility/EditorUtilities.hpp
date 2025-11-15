#pragma once
#include <concepts>
#include <limits>
#include "Utility/Math/Point2.hpp"
#include "Utility/Shapes/AABB2.hpp"
#include "Utility/ShapeMath.hpp"

namespace Simple
{
	template<std::integral T>
	[[nodiscard]] constexpr T GetInvalidIndex()
	{
		return std::numeric_limits<T>::max();
	}


	template<std::size_t Size>
	void CopyString(char(&buffer)[Size], const std::string& source)
	{
		strncpy_s(buffer, source.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';
	}


	constexpr Point2i MapToRenderRect(const Point2i mouseScreenPos, const AABB2i& renderRect)
	{
		const AABB2f windowRect = AABB2f::FromDefaultAndExtent(Vector2f(renderRect.GetExtent()));
		const Point2i mappedPos = Point2i(Remap(Point2f(mouseScreenPos), ToAABB<float>(renderRect), windowRect));
		return mappedPos;
	}

	constexpr bool IsInsideRenderRect(const Point2i& mouseScreenPos, const AABB2i& renderRect)
	{
		const AABB2f windowRect = AABB2f::FromDefaultAndExtent(Vector2f(renderRect.GetExtent()));
		const Point2i mappedPos = MapToRenderRect(mouseScreenPos, renderRect);

		return IsInside(mappedPos, ToAABB<int>(windowRect));
	}
}