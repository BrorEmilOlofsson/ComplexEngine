#pragma once
#include "Engine/Math/Shapes/Ray3.hpp"
#include "Engine/Math/VectorMath.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Math/Dimension2.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"

namespace CLX
{

	[[nodiscard]] constexpr Ray3f CalculateMouseRay(const Point3f& rayOrigin, const Matrix4x4f& viewMatrix, const Matrix4x4f& projectionMatrix, const Point2i& point, const Dimension2u& windowSize)
	{
		const Point2f clipCoords2 = ToClipCoords(point, windowSize);

		const Vector4f clipCoords(clipCoords2.x, clipCoords2.y, 1.0f, 1.0f);

		const Matrix4x4f invertedProjection = Matrix4x4f::GetInverse(projectionMatrix);

		// TODO check multiplication order
		Vector4f eyeCoords = clipCoords * invertedProjection;
		eyeCoords.z = 1.0f;
		eyeCoords.w = 0.0f;

		const Matrix4x4f invertedViewMatrix = Matrix4x4f::GetInverse(viewMatrix);
		const Vector4f rayWorld = eyeCoords * invertedViewMatrix;

		const UnitVector3f mouseDir(rayWorld.x, rayWorld.y, rayWorld.z);

		return Ray3f(rayOrigin, mouseDir);
	}

	[[nodiscard]] constexpr Ray3f CalculateMouseRay(const Camera& camera, const Point2i& point, const Dimension2u& windowSize)
	{
		return CalculateMouseRay(camera.GetPosition(), camera.GetViewMatrix(), camera.GetProjectionMatrix(), point, windowSize);
	}

	template<bool Cull = true>
	[[nodiscard]] constexpr auto GetScreenPosition(const Point3f& position, const Camera& camera, const Dimension2u& windowSize)
	{
		const Matrix4x4f viewMatrix = camera.GetViewMatrix();
		const Matrix4x4f& projectionMatrix = camera.GetProjectionMatrix();

		const Vector4f worldPosH = Vector4f(position.x, position.y, position.z, 1.0f);

		const Vector4f clipSpacePos = worldPosH * (viewMatrix * projectionMatrix);

		auto test = [clipSpacePos, windowSize]()
			{
				const float w = clipSpacePos.w;
				const Point3f ndc = Point3f(clipSpacePos.x / w, clipSpacePos.y / w, clipSpacePos.z / w);

				// Map to screen space
				const float x = (ndc.x + 1.0f) * 0.5f * static_cast<float>(windowSize.GetWidth());
				const float y = (1.0f - ndc.y) * 0.5f * static_cast<float>(windowSize.GetHeight()); // Y flipped

				return Point2f(x, y);
			};
		if constexpr (Cull)
		{
			if (clipSpacePos.w <= 0.f)
			{
				return std::optional<Point2f>(std::nullopt);
			}
			else
			{
				return std::optional<Point2f>(test());

			}
		}
		else
		{
			return test();
		}
	}
}