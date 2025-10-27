#pragma once
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Color.hpp"

namespace Simple
{
	struct Vertex final
	{
		Point3f position;
		float w = 1.f;
		Color color = Colors::White;
		Vector4f bones = Vector4f::Zero();
		Vector4f weights = Vector4f::Zero();
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