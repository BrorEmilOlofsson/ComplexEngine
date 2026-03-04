#pragma once
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Utility/Color.hpp"
#include <limits>

namespace Simple
{
	struct Vertex final
	{
		Point3f position;
		float w = 1.f;
		Color color = Colors::White;
		std::array<uint32_t, 4> bones = { std::numeric_limits<uint32_t>::max() };
		std::array<float, 4> weights = { 0.f, 0.f, 0.f, 0.f };
		UnitVector3f normal = UnitVector3f::Up();
		UnitVector3f tangent = UnitVector3f::Right();
		UnitVector3f bitangent = UnitVector3f::Forward();
		Vector2f uv = Vector2f::Zero();
	};

	static_assert(sizeof(decltype(Vertex::color)) == 16);

	struct VertexColor final
	{
		Point3f position;
		float w = 1.f;
		Color color = Colors::White;
	};

	struct VertexPosition final
	{
		Point3f position;
		float w = 1.f;
	};

	template<typename T>
	[[nodiscard]] constexpr T ToVertex(const Vertex& vertex)
	{
		if constexpr (std::same_as<T, VertexColor>)
		{
			return VertexColor
			{
				vertex.position,
				vertex.w,
				vertex.color
			};
		}
		else if constexpr (std::same_as<T, VertexPosition>)
		{
			return VertexPosition
			{
				vertex.position,
				vertex.w
			};
		}
		else
		{
			static_assert(false);
			return T{};
		}
	}
}