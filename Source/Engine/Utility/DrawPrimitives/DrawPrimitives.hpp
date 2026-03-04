#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Math/Shapes/Sphere.hpp"
#include "Engine/Math/Shapes/Cylinder.hpp"
#include "Engine/Math/Shapes/Plane.hpp"
#include "Engine/Utility/Color.hpp"

namespace CLX
{
	struct DrawBoundingBox final
	{
		Transform modelToWorld;
		AABB3f boundingBox;
		Color color;
	};

	struct DrawSphere final
	{
		Spheref sphere = Spheref::FromCenterAndRadius(Point3f::Zero(), Radiusf(1.f));
		Color color;
	};

	struct DrawArrow
	{
		Point3f startPoint;
		Point3f endPoint = Point3f(0, 1, 0);
		Color color;
		float thickness = 1.f;
	};

	struct DrawPyramid
	{
		Point3f startPoint;
		UnitVector3f direction = UnitVector3f::Up();
		Color color;
		float thickness = 1.f;
	};

	struct DrawCylinder
	{
		Cylinderf cylinder;
		Color color;
	};

	struct DrawPlane
	{
		Planef plane;
		Color color;
	};

	struct DrawLine final
	{
		Point3f startPosition;
		Point3f endPosition;
		Color color = Colors::Green;
	};
}