#pragma once
#include "Utility/CollisionFunctions.hpp"
#include "Utility/Shapes/Sphere.hpp"
#include "Utility/Math/VectorMath.hpp"
#include "Engine/Physics/PhysicsObject.hpp"
#include "Engine/Physics/Momentum.hpp"
#include "Engine/Physics/Physics.hpp"
#include "Engine/Utility/DebugShapes.hpp"

namespace Simple
{

	namespace Physics
	{

		struct CollisionResult
		{
			bool myCollided = false;
			UnitVector3f myNormal1;
			UnitVector3f myNormal2;
		};

		struct PhysicsObjectData
		{
			Velocityf myVelocity;
			Massf myMass;
			ePhysicsObjectType myType = ePhysicsObjectType::Dynamic;
		};


		inline void ResolveCollision(Spheref& sphere1, Spheref& sphere2, PhysicsObjectData& aData1, PhysicsObjectData& aData2)
		{
			const float distanceSquared = DistanceSquared(sphere1.GetCenter(), sphere2.GetCenter());
			if (distanceSquared == 0.f)
			{
				return; // avoid divide by zero
			}
			const UnitVector3f normal = GetUnitVector(sphere1.GetCenter(), sphere2.GetCenter());

			const float overlap = (sphere1.GetRadius().Value() + sphere2.GetRadius().Value()) - Sqrt(distanceSquared);

			// Push them apart based on mass ratio (to prevent overlap)
			const Massf totalMass = aData1.myMass + aData2.myMass;
			sphere1.SetCenter(sphere1.GetCenter() - normal * (overlap * (aData2.myMass / totalMass.Value())).Value());
			sphere2.SetCenter(sphere2.GetCenter() + normal * (overlap * (aData1.myMass / totalMass.Value())).Value());

			// Relative velocity
			const Velocityf relativeVelocity = aData2.myVelocity - aData1.myVelocity;

			// Only resolve if moving toward each other
			const float velocityAlongNormal = Dot(relativeVelocity.Value(), normal);
			if (velocityAlongNormal > 0)
			{
				return;
			}

			// Coefficient of restitution (1 = elastic)
			float e = 1.0f;

			float j = -(1 + e) * velocityAlongNormal;
			j /= (1 / aData1.myMass.Value()) + (1 / aData2.myMass.Value());

			const Momentum3f impulse(j * normal);

			if (aData1.myType == ePhysicsObjectType::Dynamic)
			{
				aData1.myVelocity = ApplyImpulseForce(aData1.myVelocity, -impulse, aData1.myMass);
			}
			if (aData2.myType == ePhysicsObjectType::Dynamic)
			{
				aData2.myVelocity = ApplyImpulseForce(aData2.myVelocity, impulse, aData2.myMass);
			}
		}

		inline void ResolveCollision([[maybe_unused]] Cylinderf& aCylinder1, [[maybe_unused]] Cylinderf& aCylinder2, [[maybe_unused]] PhysicsObjectData& aData1, [[maybe_unused]] PhysicsObjectData& aData2)
		{
			return;
		}


		inline void ResolveCollision(Spheref& aSphere, Cylinderf& aCylinder, PhysicsObjectData& aData1, PhysicsObjectData& aData2)
		{
			const UnitVector3f& cylinderAxis = aCylinder.GetAxis();

			const Vector3f toSphere = aSphere.GetCenter() - aCylinder.GetLowerPoint();
			const float t = Dot(toSphere, cylinderAxis);
			const Point3f closestPoint = aCylinder.GetLowerPoint() + t * cylinderAxis;

			Spheref s2 = Spheref::FromCenterAndRadius(closestPoint, aCylinder.GetRadius());

			ResolveCollision(aSphere, s2, aData1, aData2);
		}

		inline void ResolveCollision(Cylinderf& aCylinder, Spheref& aSphere, PhysicsObjectData& aData1, PhysicsObjectData& aData2)
		{
			ResolveCollision(aSphere, aCylinder, aData2, aData1);
		}
	}
}