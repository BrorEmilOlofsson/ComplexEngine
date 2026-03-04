#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Math/Shapes/Sphere.hpp"
#include "Engine/Math/Shapes/Ray3.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Math/Shapes/Disk.hpp"
#include "Engine/Math/Shapes/Plane.hpp"
#include "Engine/Math/Shapes/Cylinder.hpp"
#include "Engine/Math/Shapes/Frustrum.hpp"
#include "Engine/Math/Rotator.hpp"
#include "Engine/Utility/Color.hpp"
#include "Engine/Utility/Grid/PrimitiveGrid3.hpp"

namespace CLX
{

	class RenderList;

	void RenderSphere(const Spheref& sphere, const Color& color, RenderList& renderList);
	void RenderRay(const Ray3f& ray, const Color& color, RenderList& renderList);
	void RenderAABB3(const AABB3f& aabb, const Color& color, RenderList& renderList);
	void RenderLineSegment(const Point3f& start, const Point3f& end, const Color& color, RenderList& renderList);
	void RenderCoordinateSystem(const Point3f& aPoint, const Rotatorf& rotator, const float size, RenderList& renderList);
	void RenderCoordinateSystem(const Transform& transform, RenderList& renderList);
	void RenderDisk(const Diskf& disk, const Color& color, RenderList& renderList);
	void RenderPlane(const Planef& aPlane, const Color& color, RenderList& renderList);
	void RenderCylinder(const Cylinderf& cylinder, const Color& color, RenderList& renderList);
	void RenderGrid2(const Point2f& min, const Vector2ui& gridSize, const Vector2f& cellSize, const Vector2f& offset, const float yPos, const Color& color, RenderList& renderList);
	void RenderGrid3(const Point3f& min, const Vector3ui& gridSize, const Vector3f& cellSize, const Vector3f& offset, const Color& color, RenderList& renderList);
	void RenderGrid3(const PrimitiveGrid3& grid, const Color& color, RenderList& renderList);
	void RenderFrustrum(const Frustrumf& frustrum, const Color& color, RenderList& renderList, const bool renderFull = false);
}