#include "Engine/Precompiled/EnginePch.hpp"
#include "DebugShapes.hpp"
#include "Engine/Math/Shapes/Sphere.hpp"
#include "Engine/Math/Shapes/Ray3.hpp"
#include "Engine/Graphics/RenderList.hpp"
#include "Engine/Math/RotationMath.hpp"

namespace CLX
{

	void RenderSphere(const Spheref& sphere, const Color& color, RenderList& renderList)
	{
		DrawSphere s;
		s.color = color;
		s.sphere = sphere;
		renderList.AddSphere(s);
	}

	void RenderRay(const Ray3f& ray, const Color& color, RenderList& renderList)
	{
		DrawLine line
		{
			.startPosition = ray.GetOrigin(),
			.endPosition = ray.GetOrigin() + ray.GetDirection() * 10000.f,
			.color = color,
		};
		renderList.AddLine(line);
	}

	void RenderAABB3(const AABB3f& aabb, const Color& color, RenderList& renderList)
	{
		renderList.AddBoundingBox(DrawBoundingBox(Transform{}, aabb, color));
	}

	void RenderLineSegment(const Point3f& start, const Point3f& end, const Color& color, RenderList& renderList)
	{
		DrawLine line
		{
			.startPosition = start,
			.endPosition = end,
			.color = color,
		};
		renderList.AddLine(line);
	}

	void RenderCoordinateSystem(const Point3f& point, const RotationMatrix3f& rotationMatrix, const float size, RenderList& renderList)
	{
		const UnitVector3f xAxis = rotationMatrix.GetRight();
		const UnitVector3f yAxis = rotationMatrix.GetUp();
		const UnitVector3f zAxis = rotationMatrix.GetForward();
		RenderLineSegment(point, point + xAxis * size, Colors::Red, renderList);
		RenderLineSegment(point, point + yAxis * size, Colors::Green, renderList);
		RenderLineSegment(point, point + zAxis * size, Colors::Blue, renderList);
	}

	void RenderCoordinateSystem(const Transform& transform, RenderList& renderList)
	{
		RenderCoordinateSystem(transform.GetPosition(), transform.GetRotation(), 1.f, renderList);
	}

	void RenderDisk(const Diskf& disk, const Color& color, RenderList& renderList)
	{
		constexpr std::size_t lineCount = 20;
		constexpr Radiansf rotationAngle = ToRadians(Degreesf(360) / static_cast<float>(lineCount));

		const Point3f startPoint = disk.GetCenter() + GetPerpendicularVector(disk.GetNormal()) * disk.GetRadius().Value();
		Point3f previousPoint = startPoint;

		std::array<DrawLine, lineCount> lines{};
		for (std::size_t i = 0; i < lineCount; i++)
		{
			const Point3f endPoint = RotatePointAroundAxis(disk.GetCenter(), startPoint, disk.GetNormal(), rotationAngle * (i + 1.f));
			lines[i] = DrawLine
			{
				.startPosition = previousPoint,
				.endPosition = endPoint,
				.color = color,
			};
			previousPoint = endPoint;
		}

		renderList.AddLines(lines);
	}

	void RenderPlane(const Planef& plane, const Color& color, RenderList& renderList)
	{
		renderList.AddPlane(plane, color);
	}

	void RenderCylinder(const Cylinderf& cylinder, const Color& color, RenderList& renderList)
	{
		RenderDisk(Diskf(cylinder.GetLowerPoint(), cylinder.GetAxis(), cylinder.GetRadius()), color, renderList);
		RenderDisk(Diskf(cylinder.GetUpperPoint(), cylinder.GetAxis(), cylinder.GetRadius()), color, renderList);

		constexpr std::size_t lineCount = 20;
		constexpr Radiansf rotationAngle = ToRadians(Degreesf(360) / static_cast<float>(lineCount));

		const Point3f startPoint = cylinder.GetLowerPoint() + GetPerpendicularVector(cylinder.GetAxis()) * cylinder.GetRadius().Value();

		const Vector3f toOther = cylinder.GetAxis() * cylinder.GetHeight();

		std::array<DrawLine, lineCount> lines{};
		for (std::size_t i = 0; i < lineCount; i++)
		{
			const Point3f start = RotatePointAroundAxis(cylinder.GetLowerPoint(), startPoint, cylinder.GetAxis(), rotationAngle * (i + 1.f));
			const Point3f end = start + toOther;
			lines[i] = DrawLine
			{
				.startPosition = start,
				.endPosition = end,
				.color = color,
			};
		}
		renderList.AddLines(lines);
	}

	void RenderGrid2(const Point2f& min, const Vector2u& gridSize, const Vector2f& cellSize, const Vector2f& offset, const float yPos, const Color& color, RenderList& renderList)
	{
		std::vector<DrawLine> lines;
		lines.reserve(static_cast<std::size_t>(gridSize.x) + gridSize.y + 2);

		const float lineLengthX = cellSize.x * static_cast<float>(gridSize.x);
		const float lineLengthY = cellSize.y * static_cast<float>(gridSize.y);
		const Point2f actualMin = min - offset;
		for (unsigned int x = 0; x <= gridSize.x; x++)
		{
			const Point2f start = actualMin + RemoveY(cellSize) * static_cast<float>(x);
			const Point2f end = start + Vector2f(0, lineLengthY);

			lines.push_back(DrawLine
				{
					.startPosition = ToPoint3XZ(start, yPos),
					.endPosition = ToPoint3XZ(end, yPos),
					.color = color,
				});
		}
		for (unsigned int y = 0; y <= gridSize.y; y++)
		{
			const Point2f start = actualMin + RemoveX(cellSize) * static_cast<float>(y);
			const Point2f end = start + Vector2f(lineLengthX, 0);

			lines.push_back(DrawLine
				{
					.startPosition = ToPoint3XZ(start, yPos),
					.endPosition = ToPoint3XZ(end, yPos),
					.color = color,
				});
		}

		renderList.AddLines(lines);
	}

	void RenderGrid3(const Point3f& min, const Dimension3u& gridSize, const Dimension3f& cellSize, const Vector3f& offset, const Color& color, RenderList& renderList)
	{
		std::vector<DrawLine> lines;
		const std::size_t xCount = gridSize.GetWidth();
		const std::size_t yCount = gridSize.GetHeight();
		const std::size_t zCount = gridSize.GetDepth();

		const std::size_t estimatedLineCount =
			(xCount + 1) * (yCount + 1) +
			(xCount + 1) * (zCount + 1) +
			(yCount + 1) * (zCount + 1);

		const Point3f actualMin = min - offset;

		lines.reserve(estimatedLineCount * 2); // 2 points per line

		// Along X (for each Y-Z layer)
		for (unsigned int z = 0; z <= zCount; ++z)
		{
			for (unsigned int y = 0; y <= yCount; ++y)
			{
				const Point3f start = actualMin + Vector3f(0.0f, y * cellSize.GetHeight(), z * cellSize.GetDepth());
				const Point3f end = start + Vector3f(xCount * cellSize.GetWidth(), 0.0f, 0.0f);
				lines.emplace_back(start, end, color);
			}
		}

		// Along Y (for each X-Z layer)
		for (unsigned int z = 0; z <= zCount; ++z)
		{
			for (unsigned int x = 0; x <= xCount; ++x)
			{
				const Point3f start = actualMin + Vector3f(x * cellSize.GetWidth(), 0.0f, z * cellSize.GetDepth());
				const Point3f end = start + Vector3f(0.0f, yCount * cellSize.GetHeight(), 0.0f);
				lines.emplace_back(start, end, color);
			}
		}

		// Along Z (for each X-Y layer)
		for (unsigned int y = 0; y <= yCount; ++y)
		{
			for (unsigned int x = 0; x <= xCount; ++x)
			{
				const Point3f start = actualMin + Vector3f(x * cellSize.GetWidth(), y * cellSize.GetHeight(), 0.0f);
				const Point3f end = start + Vector3f(0.0f, 0.0f, zCount * cellSize.GetDepth());
				lines.emplace_back(start, end, color);
			}
		}


		renderList.AddLines(lines);
	}

	void RenderGrid3(const PrimitiveGrid3& grid, const Color& color, RenderList& renderList)
	{
		RenderGrid3(grid.minPos, grid.gridSize, grid.cellSize, grid.offset, color, renderList);
	}

	void RenderFrustrum(const Frustrumf& frustrum, const Color& color, RenderList& renderList, const bool renderFull)
	{
		Point3f pos = frustrum.mTransform.GetPosition();
		UnitVector3f forwardDir = frustrum.mTransform.GetForwardVector();
		UnitVector3f upDir = frustrum.mTransform.GetUpVector();
		const Radiansf halfVerticalAngle = frustrum.mVerticalAngle / 2.f;
		const Radiansf halfHorizontalAngle = frustrum.mHorizontalAngle / 2.f;
		UnitVector3f rightDir = RotateVectorAroundAxis(forwardDir, upDir, halfHorizontalAngle);
		UnitVector3f leftDir = RotateVectorAroundAxis(forwardDir, upDir, -halfHorizontalAngle);
		float nearPlane = frustrum.mNearPlaneV;
		float farPlane = frustrum.mFarPlaneV;
		const float scale = renderFull ? farPlane : Max(nearPlane + 0.1f, 3.f);

		auto createDrawLine = [color, nearPlane, scale, pos](const UnitVector3f& dir) -> DrawLine
			{
				Point3f start = pos + dir * nearPlane;
				Point3f end = pos + dir * scale;
				return DrawLine{ .startPosition = start, .endPosition = end, .color = color };
			};

		const UnitVector3f t1 = GetPerpendicularVector(leftDir, upDir);
		const UnitVector3f downLeft = RotateVectorAroundAxis(leftDir, t1, -halfVerticalAngle);
		const UnitVector3f upLeft = RotateVectorAroundAxis(leftDir, t1, halfVerticalAngle);

		renderList.AddLine(createDrawLine(downLeft));
		renderList.AddLine(createDrawLine(upLeft));


		const UnitVector3f t2 = GetPerpendicularVector(rightDir, upDir);
		const UnitVector3f downRight = RotateVectorAroundAxis(rightDir, t2, -halfVerticalAngle);
		const UnitVector3f upRight = RotateVectorAroundAxis(rightDir, t2, halfVerticalAngle);
		

		renderList.AddLine(createDrawLine(downRight));
		renderList.AddLine(createDrawLine(upRight));


		{
			Point3f downLeftPos = pos + downLeft * scale;
			Point3f downRightPos = pos + downRight * scale;
			Point3f upLeftPos = pos + upLeft * scale;
			Point3f upRightPos = pos + upRight * scale;
			renderList.AddLine(DrawLine{ downLeftPos, upLeftPos, color });
			renderList.AddLine(DrawLine{ upLeftPos, upRightPos, color });
			renderList.AddLine(DrawLine{ upRightPos, downRightPos, color });
			renderList.AddLine(DrawLine{ downRightPos, downLeftPos, color });
		}
	}
}
